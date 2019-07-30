#ifndef MTECS_WORLD_H
#define MTESC_WORLD_H

#include <stdlib.h>

#include "collection/vector.h"
#include "collection/sparse_set.h"
#include "collection/bitset.h"
#include "collection/hash_set.h"

#include "entity.h"

struct mtecs_entity_manager {
	struct hash_set entities;
	
	struct vector dead_entities;
	struct vector new_entities;
}

struct mtecs_world {

	unsigned int id;
	struct vector comp_sets;
	unsigned int comp_types;

	struct vector entities;
	struct vector destroyed_entities;
	struct vector new_entities;

	struct vector groups;
};

/* World */
void mtecs_world_init(struct mtecs_world *world, unsigned int id, unsigned int comp_types, size_t comp_sizes[]);
void mtecs_world_free(struct mtecs_world *world);

void mtecs_world_update(struct mtecs_world *world);

struct mtecs_entity_id mtecs_create_entity(struct mtecs_world *world);
void mtecs_destroy_entity(struct mtecs_world *world, struct mtecs_entity_id entity_id);

void *mtecs_add_comp(struct mtecs_world *world, struct mtecs_entity_id entity_id, unsigned int type);
void mtecs_remove_comp(struct mtecs_world *world, struct mtecs_entity_id entity_id, unsigned int type);
void *mtecs_get_comp(struct mtecs_world *world, struct mtecs_entity_id entity_id, unsigned int type);

struct mtecs_group *mtecs_get_group(struct mtecs_world *world, struct bitset mask);

#endif /* MTECS_WORLD_H */
