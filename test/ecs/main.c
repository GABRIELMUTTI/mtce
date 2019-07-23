#include <stdlib.h>
#include <stdio.h>

#include "ecs/world.h"
#include "collection/vector.h"

struct C1 {
	int id;
	int id2;
};

struct C2 {
	int a;
};

int main() {

	struct mtecs_world *world = malloc(sizeof(*world));

	size_t component_sizes[2] = {sizeof(struct C1), sizeof(struct C2)};
	
	mtecs_world_init(world, 0, 3, component_sizes);

	struct vector entities;
    	vector_init(&entities, sizeof(struct mtecs_entity*));


	for (int i = 0; i < 100; i++) {
		struct mtecs_entity *entity = mtecs_create_entity(world);
		vector_push_back(&entities, entity);

		struct C1 *c1 = mtecs_add_comp(world, entity, 0);
		
		if (i == 98) {
			c1->id = i;
			c1->id2 = c1->id * 4;				
		} else {
			c1->id = i;
			c1->id2 = c1->id * 2;			
		}	
	}

	for (unsigned int i = 0; i < world->entities.count; i++) {
		struct mtecs_entity *entity = vector_get(&world->entities, i);
		struct C1 *c1 = mtecs_get_comp(world, entity, 0);
		
		printf("Entity: %s\n", entity->name);
		printf("C1 id: %d,\tid2: %d\n", c1->id, c1->id2);
	}
    

	mtecs_world_free(world);

	free(world);
	vector_free(&entities);
    
	return 0;
}


