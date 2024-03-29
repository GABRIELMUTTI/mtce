#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

#define VECTOR_DEFAULT_ALLOC_FACTOR 2
#define VECTOR_INITIALIZER(_elem_size)					\
	(struct vector) {						\
		.array = NULL,						\
			.elem_size = _elem_size,			\
			.count = 0,					\
			.size = 0,					\
			.alloc_factor = VECTOR_DEFAULT_ALLOC_FACTOR	\
			}

struct vector {
	
	void *array;
	size_t elem_size;
	unsigned int count;
	unsigned int size;
	
	unsigned int alloc_factor;
};

struct vector *vector_init(struct vector *vector, size_t elem_size);
void vector_free(struct vector *vector);

void *vector_get(struct vector *vector, unsigned int index);
void *vector_create_back(struct vector *vector);
void *vector_sparse_create(struct vector *vector, unsigned int index);

void vector_push_back(struct vector *vector, void *elem);
void vector_set(struct vector *vector, unsigned int index, void *elem);
void vector_set_back(struct vector *vector, void *elem);
void vector_fill(struct vector *vector, void *elem);
void vector_sparse_set(struct vector *vector, unsigned int index, void *elem);

void vector_clone(struct vector *vector, unsigned int index_dest, unsigned int index_orig);
void vector_remove_back(struct vector *vector);
void vector_remove_fill(struct vector *vector, unsigned int index);
void vector_clear(struct vector *vector);
void vector_sparse_remove(struct vector *vector);

void vector_resize(struct vector *vector, unsigned int size);
void vector_set_buffer(struct vector *vector, void *buffer, unsigned int size);

#endif /* VECTOR_H */

