#include "world.h"

#include <stdio.h>

#define ENTITIES world->entities
#define DESTROYED_ENTITIES world->destroyed_entities
#define NEW_ENTITIES world->new_entities
#define GROUPS world->groups

void mtecs_world_init(struct mtecs_world *world, unsigned int id,
		      unsigned int comp_types, size_t comp_sizes[])
{
	world->id = id;
	vector_init(&world->comp_sets, sizeof(struct sparse_set));
	
	vector_init(&ENTITIES, sizeof(struct mtecs_entity));
	vector_init(&NEW_ENTITIES, sizeof(struct mtecs_entity*));
	vector_init(&DESTROYED_ENTITIES, sizeof(struct mtecs_entity*));
	
	vector_init(&GROUPS, sizeof(struct mtecs_group));


	vector_resize(&world->comp_sets, comp_types);
	
	/* Initializes the components' sparse sets. */
	for (unsigned int i = 0; i < comp_types; i++) {
		struct sparse_set *sparse_set = vector_create_back(&world->comp_sets);
		sparse_set_init(sparse_set, sizeof(comp_sizes[i]));
	}
}

void mtecs_world_free(struct mtecs_world *world)
{
	/* Frees the components. */
	for (unsigned int i = 0; i < world->comp_sets.count; i++)
		sparse_set_free(vector_get(&world->comp_sets, i));
    
	vector_free(&world->comp_sets);
	

	/* Frees the entities. */
	vector_free(&world->entities);
}

void mtecs_world_update(struct mtecs_world *world)
{

}

struct mtecs_entity_id mtecs_create_entity(struct mtecs_world *world)
{
	if (ENTITIES.count == ENTITIES.size)
		vector_resize(&ENTITIES, (ENTITIES.size + 1) * ENTITIES.alloc_factor);
		
	struct mtecs_entity *entity;
	entity = vector_create_back(&ENTITIES);

	struct mtecs_entity_id entity_id = {
		.id = 2,
		.index = 3
	};
	
	mtecs_entity_init(entity, entity_id, world->comp_types);
	return entity_id;
}

void mtecs_destroy_entity(struct mtecs_world *world, struct mtecs_entity_id entity_id)
{
	unsigned int destroyed_index = entity_id.index;
	
	vector_clone(&ENTITIES, entity_id.index, ENTITIES.count - 1);
	vector_remove_back(&ENTITIES);

	struct mtecs_entity *moved_entity = vector_get(&ENTITIES, destroyed_index);
	moved_entity->id.index = destroyed_index;
}

void *mtecs_add_comp(struct mtecs_world *world, struct mtecs_entity_id entity_id, unsigned int type)
{
	struct sparse_set *sparse_set = vector_get(&world->comp_sets, type);
	struct vector *sparse = &sparse_set->sparse;

	unsigned int index = entity_id.index;
	
	if (sparse->count <= index)
		vector_resize(sparse, (sparse->size + 1) * sparse->alloc_factor);
	
	if (index >= sparse->count) {
		sparse_set_resize_sparse(sparse_set, index + 1);
		vector_fill(sparse, (void*)-1);
	}
	
	return sparse_set_create_back(sparse_set, entity_id.index);
}

void *mtecs_get_comp(struct mtecs_world *world, struct mtecs_entity_id entity_id, unsigned int type)
{
	struct sparse_set *sparse_set = vector_get(&world->comp_sets, type);

	return sparse_set_get(sparse_set, entity_id.index);
}

void mtecs_remove_comp(struct mtecs_world *world, struct mtecs_entity_id entity_id, unsigned int type)
{
	struct sparse_set *sparse_set = vector_get(&world->comp_sets, type);

	sparse_set_remove(sparse_set, entity_id.index);
}


/* 



   ENTITIES: [e0, e1, e2,  e3,  e4,  e5, ...]
   GROUP1:   [e0, e2, e8,  e9, e11, e32, ...]
   GROUP2:   [e1, e2, e8, e14, e17, e26, ...]


   Mapping of ENTITIES -> GROUP

   entity = ENTITIES[entity_id.index]


   Group:
     vector[ENTITY]
     hash_set[Entity Id -> Index] 

     for entity in destroyed: O(DS) // OPTMIZED
         for group in member_groups[entity->index] O(DS*MG)
	     group.remove(entity)

     group.remove:
         index = hash_set.get(entity.id)
	 entities[index] = entities[count - 1]
	 entities.count--;

	 hash_set.set(entities[index].id, index);


     for entity in new: // KINDA OPTIMZED
         for group in groups:
	     if entity.mask.contains(group.mask):
	         group.add(entity)

     group.add:
         hash_set.set(entity.id, entities.count)
	 entities[entities.count] = entity;

     for entity in new_comps: // KINDA OPTIMIZED
         for group in not_member_groups:
	     if entity.mask.contains(group.mask):
	         group.add(entity)

     for entity in del_comps:  // OPTMIZED
         for group in member_groups:
	     if group.mask.get(del_comp_type):
	         group.remove(entity)

 */
