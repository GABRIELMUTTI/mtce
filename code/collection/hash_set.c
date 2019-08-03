#include "hash_set.h"

#include <string.h>

#define ENTRY_SIZE(hash_set) (sizeof(struct hash_set_entry) + hash_set->value_size + hash_set->key_size)
#define PAIR_SIZE(hash_set) (hash_set->value_size + hash_set->key_size)
#define LIST_ENTRY_SIZE(hash_set) PAIR_SIZE(hash_set)
#define ARRAY_TO_LIST_ENTRY(array) ((struct hash_set_list_entry*)array)
#define ARRAY_TO_ENTRY(array) ((struct hash_set_entry*)array)

#define GET_ENTRY(hash_set, index) (hash_set->entries + (ENTRY_SIZE(hash_set) * index))
#define GET_PAIR_KEY(hash_set, pair) (((void*)pair))
#define GET_PAIR_VALUE(hash_set, pair) (((void*)pair) + hash_set->key_size)

static int hash_set_default_key_equals(void *k1, void *k2);

static unsigned int hash_set_hash(const struct hash_set *hash_set, void *key);
static void hash_set_move_list_head(struct hash_set *hash_set, struct hash_set_entry *entry);


struct hash_set *hash_set_init(struct hash_set *hash_set, hash_fun hash, size_t key_size, size_t value_size)
{
	if (!hash_set) { hash_set = realloc(hash_set, sizeof(*hash_set)); }
	
	hash_set->value_size = value_size;
	hash_set->key_size = key_size;
	hash_set->hash = hash;
	hash_set->key_equals = hash_set_default_key_equals;
	hash_set->count = 0;
	hash_set->entries = NULL;
	hash_set->size = 0;

	return hash_set;
}

void hash_set_free(struct hash_set *hash_set)
{
	for (unsigned int i = 0; i < hash_set->size; i++) {
		struct hash_set_entry *entry = GET_ENTRY(hash_set, i);

		if (entry->exists) {
			if (entry->list != NULL) {
				linked_list_free(entry->list);
			}
		}
	}

	free(hash_set->entries);
}

void hash_set_insert(struct hash_set *hash_set, void* key, void *value)
{
	unsigned int index = hash_set_hash(hash_set, key);
	struct hash_set_entry *entry = GET_ENTRY(hash_set, index);

	if (entry->exists) {
		void *entry_key = GET_PAIR_KEY(hash_set, entry->value);
		
		/* If there's a conflict, add value to linked list */
		if (!hash_set->key_equals(key, entry_key)) {
			if (entry->list == NULL) {
				entry->list = linked_list_init(entry->list, PAIR_SIZE(hash_set), hash_set->key_equals);
			}

			char list_entry[PAIR_SIZE(hash_set)];

			memcpy(list_entry, key, hash_set->key_size);
			memcpy(list_entry + hash_set->key_size, value, hash_set->value_size);

			linked_list_push_back(entry->list, list_entry);
			
			hash_set->count++;
			return;
		}
	} else {
		entry->exists = 1;
		hash_set->count++;
	}

	entry->list = NULL;
	memcpy(entry->value, key, hash_set->key_size);
	memcpy(GET_PAIR_VALUE(hash_set, entry->value), value, hash_set->value_size);
}

void *hash_set_get(const struct hash_set *hash_set, void *key)
{
	unsigned int index = hash_set_hash(hash_set, key);
	struct hash_set_entry *entry = GET_ENTRY(hash_set, index);

	void *entry_key = GET_PAIR_KEY(hash_set, &entry->value);

	if (!hash_set->key_equals(key, entry_key)) {
		void *list_entry = linked_list_get(entry->list, key);
		return GET_PAIR_VALUE(hash_set, list_entry);
	}

	return GET_PAIR_VALUE(hash_set, &entry->value);
}

void hash_set_remove(struct hash_set *hash_set, void *key)
{
	unsigned int index = hash_set_hash(hash_set, key);
	struct hash_set_entry *entry = GET_ENTRY(hash_set, index);

	void *entry_key = GET_PAIR_KEY(hash_set, &entry->value);
	
	if (hash_set->key_equals(key, entry_key)) {
		
		if (entry->list) {
			hash_set_move_list_head(hash_set, entry);
		} else {
			entry->exists = 0;
		}	

	} else {
		linked_list_remove(entry->list, key);
	}
	
	hash_set->count--;
}

int hash_set_exists(const struct hash_set *hash_set, void *key)
{
	unsigned int index = hash_set_hash(hash_set, key);
	struct hash_set_entry *entry = GET_ENTRY(hash_set, index);

	if (!hash_set->key_equals(key, GET_PAIR_KEY(hash_set, entry->value))) {
		if (!entry->list) { return 0; }

		void *value = linked_list_get(entry->list, key);
		return value == NULL ? 0 : 1;
	}
	
	return entry->exists;
}

inline unsigned int hash_set_hash(const struct hash_set *hash_set, void *key)
{
	return hash_set->hash(key) % hash_set->size;	
}

void hash_set_resize(struct hash_set *hash_set, unsigned int size)
{
	hash_set->entries = realloc(hash_set->entries, size * ENTRY_SIZE(hash_set));
	memset(hash_set->entries + hash_set->size, 0, size - hash_set->size);
	hash_set->size = size;
}

void hash_set_move_list_head(struct hash_set *hash_set, struct hash_set_entry *entry)
{
	struct linked_list *list = entry->list;
	void *list_pair = LINKED_LIST_VALUE(list->head);
	
	memcpy(&entry->value, list_pair, PAIR_SIZE(hash_set));

	if (list->head->next) {
		list->head = list->head->next;		
	} else {
		entry->list = NULL;
		free(list);
	}
}

inline int hash_set_default_key_equals(void *k1, void *k2)
{
	return k1 == k2;
}

