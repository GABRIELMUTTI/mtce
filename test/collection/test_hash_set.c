#include "ctest.h"
#include "collection/hash_set.h"

unsigned int hash(void *key)
{
	
	return *((unsigned int*)key);
}

int equals(void *k1, void *k2)
{
	return (*(unsigned int*)k1) == (*(unsigned int*)k2);
}

typedef unsigned int key_type;
typedef float value_type;


struct hash_set *hash_set;

void setup()
{
	hash_set = hash_set_init(hash_set=NULL, hash, sizeof(key_type), sizeof(value_type));
	hash_set->key_equals = equals;
}

int test_init()
{
	CTEST_ASSERT(hash_set->size == 0);
	CTEST_ASSERT(hash_set->value_size == sizeof(value_type));
	CTEST_ASSERT(hash_set->key_size == sizeof(key_type));
	CTEST_ASSERT(hash_set->count == 0);
	CTEST_ASSERT(hash_set->hash == hash);

	return CTEST_SUCCESS;
}

int test_resize()
{
	hash_set_resize(hash_set, 100);

	CTEST_ASSERT(hash_set->size == 100);
	CTEST_ASSERT(hash_set->count == 0);

	return CTEST_SUCCESS;
}

int test_insert()
{
	hash_set_resize(hash_set, 5);
	
	value_type value = 2.0f;
	key_type key = 5;
	
	hash_set_insert(hash_set, &key, &value);

	CTEST_ASSERT(hash_set->count == 1);
	
	value_type* value_get = hash_set_get(hash_set, &key);

	CTEST_ASSERT(*value_get == value);

	return CTEST_SUCCESS;
}

int test_insert_conflict()
{
	hash_set_resize(hash_set, 1);
	
	key_type key1 = 5;
	value_type value1 = 2.0f;
	
	hash_set_insert(hash_set, &key1, &value1);
	CTEST_ASSERT(hash_set->count == 1);
	
	key_type key2 = 2;
	value_type value2 = 17.3f;

	hash_set_insert(hash_set, &key2, &value2);

	key_type key3 = 23;
	value_type value3 = 119.5f;
	
	hash_set_insert(hash_set, &key3, &value3);

	value_type* value_get1 = hash_set_get(hash_set, &key1);
	CTEST_ASSERT(*value_get1 == value1);

	value_type* value_get2 = hash_set_get(hash_set, &key2);
	CTEST_ASSERT(*value_get2 == value2);

	value_type* value_get3 = hash_set_get(hash_set, &key3);
	CTEST_ASSERT(*value_get3 == value3);

	return CTEST_SUCCESS;
}

int test_remove_from_table()
{
	hash_set_resize(hash_set, 1);

	key_type key1 = 1;
	value_type value1 = 15.0f;

	key_type key2 = 2;
	value_type value2 = 16.0f;

	key_type key3 = 3;
	value_type value3 = 17.0f;

	hash_set_insert(hash_set, &key1, &value1);
	hash_set_insert(hash_set, &key2, &value2);
	hash_set_insert(hash_set, &key3, &value3);

	hash_set_remove(hash_set, &key1);

	value_type* value2_get = hash_set_get(hash_set, &key2);
	value_type* value3_get = hash_set_get(hash_set, &key3);

	CTEST_ASSERT(*value2_get == value2);
	CTEST_ASSERT(*value3_get == value3);

	int key1_exists = hash_set_exists(hash_set, &key1);
	CTEST_ASSERT(!key1_exists);
	
	return CTEST_SUCCESS;
}

int test_remove_from_list()
{
	hash_set_resize(hash_set, 1);

	key_type key1 = 1;
	value_type value1 = 15.0f;

	key_type key2 = 2;
	value_type value2 = 16.0f;

	key_type key3 = 3;
	value_type value3 = 17.0f;

	hash_set_insert(hash_set, &key1, &value1);
	hash_set_insert(hash_set, &key2, &value2);
	hash_set_insert(hash_set, &key3, &value3);

	hash_set_remove(hash_set, &key2);

	value_type* value1_get = hash_set_get(hash_set, &key1);
	value_type* value3_get = hash_set_get(hash_set, &key3);

	CTEST_ASSERT(*value1_get == value1);
	CTEST_ASSERT(*value3_get == value3);

	int key2_exists = hash_set_exists(hash_set, &key2);
	CTEST_ASSERT(!key2_exists);

	return CTEST_SUCCESS;
}

int main() {

	struct ctest_suite suite = CTEST_SUITE_INITIALIZER("hash_set");
	suite.setup_function = setup;
	
	ctest_suite_add_case(&suite, "hash_set_init", test_init);
	ctest_suite_add_case(&suite, "hash_set_resize", test_resize);
	ctest_suite_add_case(&suite, "hash_set_insert", test_insert);
	ctest_suite_add_case(&suite, "hash_set_insert_conflict", test_insert_conflict);
	ctest_suite_add_case(&suite, "hash_set_remove_from_table", test_remove_from_table);
	ctest_suite_add_case(&suite, "hash_set_remove_from_list", test_remove_from_list);

	ctest_suite_run(&suite);
	
	return 0;
}
