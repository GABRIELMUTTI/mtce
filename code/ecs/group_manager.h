#ifndef MTECS_GROUP_MANAGER_H
#define MTECS_GROUP_MANAGER_H

#include "collection/vector.h"
#include "collection/bitset.h"

struct mtecs_group {
	struct vector entities;
	struct bitset comp_mask;
};

struct mtecs_group_manager {

	struct vector groups;
	
};

#endif	/* MTECS_GROUP_MANAGER_H */
