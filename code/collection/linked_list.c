#include "linked_list.h"

#include <string.h>

#define ENTRY_SIZE(linked_list) (sizeof(struct linked_list) + linked_list->value_size)

struct linked_list *linked_list_init(size_t value_size)
{
	struct linked_list *linked_list = malloc(sizeof(struct linked_list) + value_size);
	linked_list->next = NULL;
	linked_list->value_size = value_size;
	linked_list->has_value = 0;

	return linked_list;
}

void linked_list_free(struct linked_list *linked_list)
{
	struct linked_list *cur = linked_list;
	struct linked_list *next = NULL;
	while (cur != NULL) {
		next = cur->next;
		free(cur);
		cur = next;
	}
}

void linked_list_push_back(struct linked_list *linked_list, void *value)
{
	if (!linked_list->has_value) {
		linked_list->has_value = 1;
		memcpy(&linked_list->value, value, linked_list->value_size);
		
	} else {
		struct linked_list *tail = linked_list_tail(linked_list);
       	
		tail->next = linked_list_init(tail->value_size);

		tail->next->has_value = 1;
		memcpy(&tail->next->value, value, tail->value_size);
	}
}

void linked_list_index_remove(struct linked_list *linked_list, unsigned int index)
{
	int found = 0;
	unsigned int cur_index = 0;
	struct linked_list *cur = linked_list;
	struct linked_list *prev = NULL;
	while (cur != NULL && cur_index != index) {

		if (cur_index == index) {
			found = 1;
			break;			
		}

		prev = cur;
		cur = cur->next;
	}

	if (found && cur != NULL) {
		if (prev) {
			prev->next = cur->next;			
		}

		linked_list_free(cur);
	}
}

struct linked_list *linked_list_tail(struct linked_list *linked_list)
{
	struct linked_list *cur = linked_list;
	while (cur->next != NULL) {
		cur = cur->next;
	}

	return cur;
}

/* Has memory leak on value */
void linked_list_remove(struct linked_list *linked_list, void *value)
{
	struct linked_list *prev = NULL;
	struct linked_list *cur = linked_list;
	while (cur != NULL) {
		if (cur->equal_fun(&cur->value, value)) {
			if (prev) {
				prev->next = cur->next;
			}

			linked_list_free(cur);
			return;
		}
	}
}

