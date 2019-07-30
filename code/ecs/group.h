#ifndef MTECS_GROUP_H
#define MTECS_GROUP_H

#include "collection/vector.h"
#include "collection/bitset.h"

#include "entity.h"

struct mtecs_member_info {
	struct mtecs_entity_id entity_id;
	unsigned int member_index;
};

struct mtecs_group {

	struct vector entities;
	struct vector member_infos;
	struct bitset comp_mask;
};

struct mtecs_group *mtecs_group_init(struct mtecs_group *group);

void mtecs_group_add(struct mtecs_group *group, struct mtecs_entity *entity, struct mtecs_member_info member_info);
void mtecs_group_remove(struct mtecs_group *group, struct mtecs_entity *entity, struct mtecs_member_info member_info);
struct mtecs_entity_id *mtecs_group_get(struct mtecs_group *group, unsigned int index);


#endif /* MTECS_GROUP_H */

