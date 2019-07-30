#ifndef SPARSE_SET_H
#define SPARSE_SET_H

#include <stdlib.h>

#include "vector.h"

struct sparse_set {
	struct vector dense;
	struct vector sparse;
};

void sparse_set_init(struct sparse_set *sparse_set, size_t obj_size);
void sparse_set_free(struct sparse_set *sparse_set);

void sparse_set_push_back(struct sparse_set *sparse_set, void *obj, int id);
void sparse_set_set(struct sparse_set *sparse_set, void *obj, int id);

void* sparse_set_get(struct sparse_set *sparse_set, int id);
void* sparse_set_get_dense(struct sparse_set *sparse_set, unsigned int index);
void* sparse_set_create_back(struct sparse_set *sparse_set, int id);

void sparse_set_remove(struct sparse_set *sparse_set, int id);

void sparse_set_resize_dense(struct sparse_set *sparse_set, unsigned int size);
void sparse_set_resize_sparse(struct sparse_set *sparse_set, unsigned int size);

#endif /* SPARSE_SET_H */

