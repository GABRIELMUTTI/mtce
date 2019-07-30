#ifndef COL_PAIR_H
#define COL_PAIR_H

#include <stdlib.h>

struct pair {
	void *first;
	void *second;
};

struct pair *pair_init(struct pair *pair, void *first, void *second, size_t size_first, size_t size_second);
void pair_free(struct pair *pair);

#endif /* COL_PAIR_H */
