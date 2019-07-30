#ifndef MTECS_ENTITY_H
#define MTECS_ENTITY_H

#include "collection/bitset.h"

typedef unsigned int entity_id;

struct mtecs_entity {

	entity_id id;
	struct bitset archetype;
};

struct mtecs_entity *mtecs_entity_init(struct mtecs_entity *entity);

#endif	/* MTECS_ENTITY_H */

