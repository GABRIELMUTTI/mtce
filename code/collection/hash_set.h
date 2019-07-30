#ifndef COL_HASH_SET_H
#define COL_HASH_SET_H

#include <stdlib.h>

#include "vector.h"
#include "linked_list.h"

typedef unsigned int(*hash_fun)(void*);
typedef int(*equal_fun)(void*, void*);

struct hash_set_entry {
	int exists;
	unsigned int key_index;
	struct linked_list* list;
	char value[];
};

struct hash_set_list_entry {
	unsigned int key_index;
	char value[];
};

struct hash_set {

	struct vector keys;
	struct vector entries;
	size_t value_size;
	unsigned int count;

	
	hash_fun hash_fun;
	equal_fun key_equal_fun;
};

struct hash_set *hash_set_init(struct hash_set *hash_set, hash_fun hash_fun, size_t key_size, size_t value_size);
void hash_set_free(struct hash_set *hash_set);

void hash_set_insert(struct hash_set *hash_set, void *key, void *value);
void *hash_set_get(struct hash_set *hash_set, void *key);
void hash_set_remove(struct hash_set *hash_set, void *key);

int hash_set_exists(struct hash_set *hash_set, void *key);

void hash_set_resize(struct hash_set *hash_set, unsigned int size);

#endif /* HASH_SET_H */

