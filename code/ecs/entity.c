#include "entity.h"

#include <string.h>
#include <stdlib.h>

struct mtecs_entity *mtecs_entity_init(struct mtecs_entity *entity, unsigned int id, const char *name, unsigned int num_comp_types)
{
    entity->id = id;
    strcpy(entity->name, name);

    mtecs_mask_init(&entity->_mask, num_comp_types);

    return entity;
}

inline void mtecs_entity_free(struct mtecs_entity *entity)
{
     mtecs_mask_free(&entity->_mask);
}

inline void mtecs_entity_add_comp_mask(struct mtecs_entity *entity, unsigned int comp_id)
{
    mtecs_mask_set(&entity->_mask, comp_id, 1);
}

inline void mtecs_entity_remove_comp_mask(struct mtecs_entity *entity, unsigned int comp_id)
{
    mtecs_mask_set(&entity->_mask, comp_id, 0);
}

inline void mtecs_entity_set_name(struct mtecs_entity *entity, const char* name)
{
    free(entity->name);
    strcpy(entity->name, name);
}

inline int mtecs_entity_has_comp(struct mtecs_entity *entity, unsigned int comp_id)
{
    return mtecs_mask_get(&entity->_mask, comp_id);
}
