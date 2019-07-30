#include "group.h"

struct mtecs_group *mtecs_group_init(struct mtecs_group *group)
{
	vector_init(&group->entities, sizeof(struct mtecs_entity));
	bitset_init(&group->comp_mask, sizeof(struct bitset));
	
	return group;
}

void mtecs_group_add(struct mtecs_group *group, struct mtecs_entity *entity,
		     struct mtecs_member_info *entity_group_info)
{
	entity_group_info->index = group->entities.count;
	vector_push_back(&group->entities, entity);
}

void mtecs_group_remove(struct mtecs_group *group, struct mtecs_entity *entity, struct mtecs_entity_group_info *entity_group_info)
{
	vector_remove_fill(&group->entities, entity_group_info->index);
	
	struct mtecs_entity *moved = vector_get(&group->entities, entity_group_info->index);

	hash_set_get(&moved->group_infos, group->id);



	
}


inline struct mtecs_entity_id *mtecs_group_get(struct mtecs_group *group, unsigned int index)
{
	return vector_get(&group->entities, index);
}
