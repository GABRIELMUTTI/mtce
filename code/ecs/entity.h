#ifndef MTECS_ENTITY_H
#define MTECS_ENTITY_H

#include "mask.h"

#define MTECS_ENTITY_NAME_SIZE 256

struct mtecs_entity {
    unsigned int id;

    char name[MTECS_ENTITY_NAME_SIZE];
    struct mtecs_mask _mask;
};


struct mtecs_entity *mtecs_entity_init(struct mtecs_entity *entity, unsigned int id, const char *name, unsigned int num_comp_types);
void mtecs_entity_free(struct mtecs_entity *entity);

void mtecs_entity_add_comp_mask(struct mtecs_entity *entity, unsigned int comp_id);
void mtecs_entity_remove_comp_mask(struct mtecs_entity *entity, unsigned int comp_id);
int mtecs_entity_has_comp(struct mtecs_entity *entity, unsigned int comp_id);

void mtecs_entity_set_name(struct mtecs_entity *entity, const char* name);


#endif /* MTECS_ENTITY_H */

