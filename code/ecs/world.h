#ifndef MTECS_WORLD_H
#define MTESC_WORLD_H

#include <stdlib.h>

#include "collection/vector.h"
#include "collection/sparse_set.h"
#include "entity.h"


struct mtecs_world {

	unsigned int id;
	struct vector component_sets;
	struct vector entities;

	unsigned int num_component_types;
	unsigned int num_entities;
};

void mtecs_world_init(struct mtecs_world *world, unsigned int id, unsigned num_component_type, size_t component_sizes[]);
void mtecs_world_free(struct mtecs_world *world);

struct mtecs_entity *mtecs_create_entity(struct mtecs_world* world);

void *mtecs_add_comp(struct mtecs_world* world, struct mtecs_entity* entity, unsigned int type);
void mtecs_remove_comp(struct mtecs_world* world, struct mtecs_entity* entity, unsigned int type);
void *mtecs_get_comp(struct mtecs_world* world, struct mtecs_entity* entity, unsigned int type); 

#endif /* MTECS_WORLD_H */
