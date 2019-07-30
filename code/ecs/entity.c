#include "entity.h"

struct mtecs_entity *mtecs_entity_init(struct mtecs_entity *entity)
{
	entity->id = 0;
	bitset_init(&entity->archetype);
	
	return entity;
}
