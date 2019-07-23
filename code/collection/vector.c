#include "vector.h"

#include <string.h>

/* Resizes the factor based on the allocation factor.  */
static void vector_factor_resize(struct vector *vector);

void vector_init(struct vector *vector, size_t elem_size)
{
	vector->array = NULL;
	vector->count = 0;
	vector->elem_size = elem_size;
	vector->size = 0;
	vector->alloc_factor = VECTOR_DEFAULT_ALLOC_FACTOR;
}

inline void vector_free(struct vector *vector)
{
	free(vector->array);
}

void vector_push_back(struct vector *vector, void *elem)
{    
	if (vector->count == vector->size)
		vector_factor_resize(vector);

	void *start = VECTOR_GET_ELEM(vector, vector->count);
	memcpy(start, elem, vector->elem_size);

	vector->count++;
}

inline void *vector_get(struct vector *vector, unsigned int index)
{
	return VECTOR_GET_ELEM(vector, index);
}

void vector_set(struct vector *vector, unsigned int index, void *elem)
{
	void *start = VECTOR_GET_ELEM(vector, index);
	memcpy(start, elem, vector->elem_size);
}

inline void vector_set_back(struct vector *vector, void * elem)
{
	vector_set(vector, vector->count, elem);
}

static void vector_factor_resize(struct vector *vector)
{
	unsigned int size;
	if (vector->size == 0) {
		size = vector->alloc_factor;
	} else {
		size = vector->alloc_factor * vector->size;
	}
    
	vector_resize(vector, size);    
}

void *vector_create_back(struct vector *vector)
{
	if (vector->count == vector->size)
		vector_factor_resize(vector);
	
    	void *elem = VECTOR_GET_ELEM(vector, vector->count);
	vector->count++;

	return elem;
}

void vector_clone(struct vector *vector, unsigned int index_dest, unsigned int index_orig)
{
	void *dest = VECTOR_GET_ELEM(vector, index_dest);
	void *orig = VECTOR_GET_ELEM(vector, index_orig);
	memcpy(dest, orig, vector->elem_size);
}

inline void vector_remove_back(struct vector *vector)
{
	vector->count--;
}

void vector_fill(struct vector *vector, void *elem)
{
	for (unsigned int i = vector->count; i < vector->size; i++) {
		void *elem_start = VECTOR_GET_ELEM(vector, i);
		memcpy(elem_start, elem, vector->elem_size);
		vector->count++;
	}
}

void vector_resize(struct vector *vector, unsigned int size)
{
	vector->array = realloc(vector->array, vector->elem_size * size);

	if (vector->size > size)
		vector->count = size;

	vector->size = size;
}

void vector_set_buffer(struct vector *vector, void *buffer, unsigned int size)
{
	free(vector->array);
	vector->array = buffer;
	vector->size = size;
	vector->count = 0;
}

inline void vector_clear(struct vector *vector)
{
	vector->count = 0;
}

