
#include <stdio.h>
#include <stdlib.h>

#include "collection/hash_set.h"

struct key_type {
	unsigned int a;
	int b;
};

struct value_type {
	unsigned int a;
	float b;
};

int key_equal(void *a, void *b)
{
	struct key_type *ka = a;
	struct key_type *kb = b;
	
	return ka->a == kb->a && ka->b == kb->b;
}

unsigned int hash_function(void *key)
{
	struct key_type *k = key;
	return (unsigned int)((k->a << 4) + (k->b * 5));
}

int main()
{
	struct hash_set hash_set;
	hash_set_init(&hash_set, hash_function, sizeof(struct key_type), sizeof(struct value_type));
	hash_set.key_equal_fun = key_equal;
	
	hash_set_resize(&hash_set, 10000);

	for (unsigned int i = 0; i < 100000; i++) {
		struct key_type k = {
			.a = 1 + i,
			.b = 2 + i
		};
		
		struct value_type v = {
		.a = i,
		.b = 53.f + (float)i
	};

		hash_set_insert(&hash_set, &k, &v);
	}


	printf("hash_set_count: %d\n", hash_set.count);

	for (int i = 0; i < 100000; i++) {


		
		struct key_type k = {
			.a = 1 + i,
			.b = 2 + i
		};

		struct value_type *k1_get = hash_set_get(&hash_set, &k);
		
///		printf("k: %d, %d \t-> \t%d, %f\n", k.a, k.b, k1_get->a, k1_get->b);
		hash_set_remove(&hash_set, &k);
		

	}

	printf("hash_set_count: %d\n", hash_set.count);

	hash_set_free(&hash_set);

	printf("done.\n");
	
}
