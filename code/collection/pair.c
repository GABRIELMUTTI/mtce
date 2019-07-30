#include "pair.h"

#include <string.h>

struct pair *pair_init(struct pair *pair, void *first, void *second, size_t size_first, size_t size_second)
{
	pair->first = malloc(size_first + size_second);

	memcpy(pair->first, first, size_first);
	pair->second = memcpy(pair->first + size_first, second, size_second);

	return pair;
}

inline void pair_free(struct pair *pair)
{
	free(pair->first);
}
