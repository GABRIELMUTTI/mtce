#ifndef COL_HASH_SET_H
#define COL_HASH_SET_H

#include <stdlib.h>

#include "vector.h"
#include "linked_list.h"

#define HASH_SET_INITIALIZER(_key_size, _value_size, _hash) \
	(struct hash_set) {						\
		.entries = NULL,					\
			.size = 0,					\
			.value_size = _value_size,			\
			.key_size = _key_size,				\
			.count = 0,					\
			.hash = _hash,					\
			}							     

typedef unsigned int(*hash_fun)(void*);
typedef int(*equals_fun)(void*, void*);

struct hash_set_entry {
	
	int exists;
	struct linked_list* list;
	char value[];
};

struct hash_set {

	void *entries;
	unsigned int size;
	size_t value_size;
	size_t key_size;
	unsigned int count;

	hash_fun hash;
	equals_fun key_equals;
};

struct hash_set *hash_set_init(struct hash_set *hash_set, hash_fun hash_fun, size_t key_size, size_t value_size);
void hash_set_free(struct hash_set *hash_set);

void hash_set_insert(struct hash_set *hash_set, void *key, void *value);
void *hash_set_get(const struct hash_set *hash_set, void *key);
void hash_set_remove(struct hash_set *hash_set, void *key);

int hash_set_exists(const struct hash_set *hash_set, void *key);

void hash_set_resize(struct hash_set *hash_set, unsigned int size);

#endif /* HASH_SET_H */

