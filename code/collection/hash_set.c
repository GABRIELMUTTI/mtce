#include "hash_set.h"

#include <string.h>

#define ENTRY_SIZE(hash_set) (sizeof(struct hash_set_entry) + hash_set->value_size)
#define LIST_ENTRY_SIZE(hash_set) ((sizeof(struct hash_set_list_entry) + hash_set->value_size))
#define ARRAY_TO_LIST_ENTRY(array) ((struct hash_set_list_entry*)array)
#define ARRAY_TO_ENTRY(array) ((struct hash_set_entry*)array)

static int hash_set_default_key_equal_fun(void *k1, void *k2);
static int hash_set_default_list_entry_equal_fun(void *e1, void *e2);

static unsigned int hash_set_hash(struct hash_set *hash_set, void *key);
static void *hash_set_list_get(struct hash_set *hash_set, struct linked_list *list, void *key);
static void hash_set_free_list(struct linked_list* list);
static void hash_set_list_remove(struct hash_set *hash_set, struct hash_set_entry *entry, void *key);
static void hash_set_move_list_head(struct hash_set *hash_set, struct hash_set_entry *entry);


struct hash_set *hash_set_init(struct hash_set *hash_set, hash_fun hash_fun, size_t key_size, size_t value_size)
{
	hash_set->value_size = value_size;
	hash_set->hash_fun = hash_fun;
	hash_set->key_equal_fun = hash_set_default_key_equal_fun;
	hash_set->count = 0;
	
	vector_init(&hash_set->keys, key_size);
	vector_init(&hash_set->entries, ENTRY_SIZE(hash_set));

	return hash_set;
}

void hash_set_free(struct hash_set *hash_set)
{
	for (unsigned int i = 0; i < hash_set->entries.size; i++) {
		struct hash_set_entry *entry = vector_get(&hash_set->entries, i);

		if (entry->exists) {
			if (entry->list != NULL) {
				hash_set_free_list(entry->list);
			}
		}
	}

	vector_free(&hash_set->entries);
	vector_free(&hash_set->keys);
}

void hash_set_insert(struct hash_set *hash_set, void* key, void *value)
{
	unsigned int index = hash_set_hash(hash_set, key);
	struct hash_set_entry *entry = vector_get(&hash_set->entries, index);

	if (entry->exists) {

		/* If there's a conflict, add value to linked list */
		if (!hash_set->key_equal_fun(key, vector_get(&hash_set->entries, entry->key_index))) {
			if (entry->list == NULL) {
				entry->list = linked_list_init(LIST_ENTRY_SIZE(hash_set));
				entry->list->equal_fun = hash_set_default_list_entry_equal_fun;
			}


			char list_entry_data[sizeof(struct hash_set_list_entry) + hash_set->value_size];
			struct hash_set_list_entry *list_entry = ARRAY_TO_LIST_ENTRY(list_entry_data);
			
			list_entry->key_index = hash_set->keys.count;
			memcpy(&list_entry->value, value, hash_set->value_size);
			
			linked_list_push_back(entry->list, list_entry);
			vector_push_back(&hash_set->keys, key);
			
			hash_set->count++;
			return;
		}
	} else {
		entry->exists = 1;
		hash_set->count++;
	}

	entry->key_index = hash_set->keys.count;
	entry->list = NULL;
	memcpy(&entry->value, value, hash_set->value_size);
	
	vector_push_back(&hash_set->keys, key);
}

void *hash_set_get(struct hash_set *hash_set, void *key)
{
	unsigned int index = hash_set_hash(hash_set, key);
	struct hash_set_entry *entry = vector_get(&hash_set->entries, index);

	void *entry_key = vector_get(&hash_set->keys, entry->key_index);

	if (!hash_set->key_equal_fun(key, entry_key)) {
		return hash_set_list_get(hash_set, entry->list, key);		
	}

	return entry->value;
}

void hash_set_remove(struct hash_set *hash_set, void *key)
{
	unsigned int index = hash_set_hash(hash_set, key);
	struct hash_set_entry *entry = vector_get(&hash_set->entries, index);
	unsigned int removed_entry_key_index = entry->key_index;

	void *entry_key = vector_get(&hash_set->keys, entry->key_index);
	if (!hash_set->key_equal_fun(entry_key, key)) {
		hash_set_list_remove(hash_set, entry, key);
	} else {
		if (entry->list) {
			hash_set_move_list_head(hash_set, entry);
		} else {
			entry->exists = 0;
		}		
	}
	
	hash_set->count--;
	
	/* Removes key from key list */
	
	vector_remove_fill(&hash_set->keys, removed_entry_key_index);

	/* Updates swapped key's entry key index */
	void *swapped_key = vector_get(&hash_set->keys,  removed_entry_key_index);
	unsigned int swapped_key_entry_index = hash_set_hash(hash_set, swapped_key);
	struct hash_set_entry *swapped_key_entry = vector_get(&hash_set->entries, swapped_key_entry_index);

	if (hash_set->key_equal_fun(swapped_key, vector_get(&hash_set->keys, swapped_key_entry->key_index))) {
		swapped_key_entry->key_index = removed_entry_key_index;		
	} else {
		struct linked_list *cur = swapped_key_entry->list;

		while (cur != NULL) {
			struct hash_set_list_entry *list_entry = LINKED_LIST_VALUE(cur);
			
			if (hash_set->key_equal_fun(swapped_key, vector_get(&hash_set->keys, list_entry->key_index))) {
				list_entry->key_index = removed_entry_key_index;
				return;
			}

			cur = cur->next;
		}
	}
}

int hash_set_exists(struct hash_set *hash_set, void *key)
{
	unsigned int index = hash_set_hash(hash_set, key);
	struct hash_set_entry *entry = vector_get(&hash_set->entries, index);
	return entry->exists;
}

inline unsigned int hash_set_hash(struct hash_set *hash_set, void *key)
{
	return hash_set->hash_fun(key) % hash_set->entries.size;	
}

void *hash_set_list_get(struct hash_set *hash_set, struct linked_list *list, void *key)
{
	struct linked_list *cur = list;
	while (cur != NULL) {
		struct hash_set_list_entry *list_entry = LINKED_LIST_VALUE(cur);
		void *list_key = vector_get(&hash_set->keys, list_entry->key_index);

		if (hash_set->key_equal_fun(key, list_key)) {
			return list_entry->value;
		}

		cur = cur->next;
	}

	return NULL;
}

inline int hash_set_default_key_equal_fun(void *k1, void *k2)
{
	return k1 == k2;
}

inline int hash_set_default_list_entry_equal_fun(void *e1, void *e2)
{
	return ((struct hash_set_list_entry*)e1)->key_index == ((struct hash_set_list_entry*)e2)->key_index;
}
	
void hash_set_resize(struct hash_set *hash_set, unsigned int size)
{
	vector_resize(&hash_set->entries, size);

	char empty_entry_data[ENTRY_SIZE(hash_set)];
	struct hash_set_entry *empty_entry = ARRAY_TO_ENTRY(empty_entry_data);
	empty_entry->exists = 0;
	empty_entry->key_index = 0;
	empty_entry->list = NULL;

	vector_fill(&hash_set->entries, empty_entry);
}

void hash_set_free_list(struct linked_list* list)
{
	struct linked_list *cur = list;
	struct linked_list *tmp = NULL;

	while (cur != NULL) {
		free(cur->value);
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
}

void hash_set_list_remove(struct hash_set *hash_set, struct hash_set_entry *entry, void *key)
{
	struct linked_list* cur = entry->list;
	struct linked_list* prev = NULL;
	
	while (cur != NULL) {
		struct hash_set_list_entry *list_entry = LINKED_LIST_VALUE(cur);
		void *list_key = vector_get(&hash_set->keys, list_entry->key_index);

		if (hash_set->key_equal_fun(key, list_key)) {
			if (prev) {
				prev->next = cur->next;
			} else {
				entry->list = cur->next;
			}

			free(cur->value);
			free(cur);
			return;
		}

		prev = cur;
		cur = cur->next;
	}
}

void hash_set_move_list_head(struct hash_set *hash_set, struct hash_set_entry *entry)
{
	struct linked_list *list = entry->list;
	struct hash_set_list_entry *list_entry = LINKED_LIST_VALUE(list);

	entry->exists = 1;
	entry->key_index = list_entry->key_index;
	memcpy(&entry->value, &list_entry->value, hash_set->value_size);
	entry->list = list->next;

	free(list);
}
