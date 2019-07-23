#include "sparse_set.h"

#include <string.h>

inline void sparse_set_init(struct sparse_set *sparse_set, size_t obj_size)
{
	vector_init(&sparse_set->dense, obj_size);
	vector_init(&sparse_set->sparse, sizeof(int));
}

inline void sparse_set_free(struct sparse_set *sparse_set) {
	vector_free(&sparse_set->dense);
	vector_free(&sparse_set->sparse);
}

void sparse_set_push_back(struct sparse_set *sparse_set, void *obj, int id)
{
	vector_push_back(&sparse_set->dense, obj);

	/* Allocates more space for the sparse if necessary. */
	unsigned int index = sparse_set->dense.count - 1;
	if (index >= sparse_set->sparse.size) {
		vector_resize(&sparse_set->sparse, index);
		vector_fill(&sparse_set->sparse, (void*)-1);
	}
    
	vector_set(&sparse_set->sparse, id, &index);
}

inline void *sparse_set_get(struct sparse_set *sparse_set, int id)
{
	unsigned int *index = vector_get(&sparse_set->sparse, id);
	return vector_get(&sparse_set->dense, *index);
}

inline void *sparse_set_get_dense(struct sparse_set *sparse_set, unsigned int index)
{
	return vector_get(&sparse_set->dense, index);
}

inline void sparse_set_set(struct sparse_set *sparse_set, void *obj, int id)
{
	unsigned int *index = vector_get(&sparse_set->sparse, id);
	vector_set(&sparse_set->dense, *index, obj);
}

inline void *sparse_set_create_back(struct sparse_set *sparse_set, int id)
{
	vector_set(&sparse_set->sparse, id, &sparse_set->dense.count);
	return vector_create_back(&sparse_set->dense);
}

void sparse_set_remove(struct sparse_set *sparse_set, int id)
{
	vector_set(&sparse_set->sparse, id, (void*)-1);

	int *dense_index = vector_get(&sparse_set->sparse, id);
	vector_clone(&sparse_set->dense, *dense_index, sparse_set->dense.count);
	vector_remove_back(&sparse_set->dense);
}

void sparse_set_resize_dense(struct sparse_set *sparse_set, unsigned int count)
{
	vector_resize(&sparse_set->dense, count);

	if (sparse_set->sparse.size < count)
		vector_resize(&sparse_set->sparse, count);
}

inline void sparse_set_resize_sparse(struct sparse_set *sparse_set, unsigned int count)
{
	vector_resize(&sparse_set->sparse, count);
}
