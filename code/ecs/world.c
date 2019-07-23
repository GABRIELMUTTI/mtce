#include "world.h"

#include <stdio.h>

#define MTECS_ENTITY_NAME_SIZE 256

static const char MTECS_ENTITY_DEFAULT_NAME_PREFIX[] = "Entity #";

static void free_components(struct mtecs_world *world);
static void free_entities(struct mtecs_world *world);

void mtecs_world_init(struct mtecs_world *world, unsigned int id, unsigned num_component_types, size_t component_sizes[])
{
	world->id = id;
	world->num_entities = 0;
	vector_init(&world->component_sets, sizeof(struct sparse_set));
	vector_init(&world->entities, sizeof(struct mtecs_entity));

	vector_resize(&world->component_sets, num_component_types);
	
	/* Initializes the components' sparse sets. */
	for (unsigned int i = 0; i < num_component_types; i++) {
		struct sparse_set *sparse_set = vector_create_back(&world->component_sets);
		sparse_set_init(sparse_set, sizeof(component_sizes[i]));
	}
}

void mtecs_world_free(struct mtecs_world *world)
{
	free_components(world);
	free_entities(world);
}

static void free_components(struct mtecs_world *world)
{
	for (unsigned int i = 0; i < world->component_sets.count; i++)
		sparse_set_free(vector_get(&world->component_sets, i));
    
	vector_free(&world->component_sets);
}

static void free_entities(struct mtecs_world *world)
{
	for (unsigned int i = 0; i < world->entities.count; i++)
		mtecs_entity_free(vector_get(&world->entities, i));

	vector_free(&world->entities);
}

struct mtecs_entity *mtecs_create_entity(struct mtecs_world *world)
{
	if (world->entities.count == world->entities.size)
		vector_resize(&world->entities, (world->entities.size + 1) * world->entities.alloc_factor);
		
	struct mtecs_entity *entity;
	entity = vector_create_back(&world->entities);

	char name[MTECS_ENTITY_NAME_SIZE];
	sprintf(name, "%s%d", MTECS_ENTITY_DEFAULT_NAME_PREFIX, world->num_entities);
    
	mtecs_entity_init(entity, world->num_entities, name, world->num_component_types);

	world->num_entities++;

	return entity;
}

void *mtecs_add_comp(struct mtecs_world *world, struct mtecs_entity* entity, unsigned int type)
{
	struct sparse_set *sparse_set = vector_get(&world->component_sets, type);
	struct vector *sparse = &sparse_set->sparse;

	if (sparse->count <= entity->id)
		vector_resize(sparse, (sparse->size + 1) * sparse->alloc_factor);
	
	if (entity->id >= sparse->count) {
		sparse_set_resize_sparse(sparse_set, entity->id + 1);
		vector_fill(sparse, (void*)-1);
	}
	
	return sparse_set_create_back(sparse_set, entity->id);
}

void *mtecs_get_comp(struct mtecs_world *world, struct mtecs_entity* entity, unsigned int type)
{
	struct sparse_set *sparse_set = vector_get(&world->component_sets, type);

	return sparse_set_get(sparse_set, entity->id);
}

void mtecs_remove_comp(struct mtecs_world *world, struct mtecs_entity* entity, unsigned int type)
{
	struct sparse_set *sparse_set = vector_get(&world->component_sets, type);

	sparse_set_remove(sparse_set, entity->id);
}


