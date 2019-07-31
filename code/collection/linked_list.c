#include "linked_list.h"

#define LIST_NODE_SIZE(list) (sizeof(struct list_node) + list->value_size)

#include <string.h>





struct linked_list *linked_list_init(struct linked_list* list, size_t value_size, equals_fun equals)
{
	if (!list) { list = malloc(sizeof(*list)); }
	
	list->head = NULL;
	list->tail = NULL;
	list->value_size = value_size;
	list->count = 0;
	list->equals = equals;

	return list;
}

void linked_list_free(struct linked_list *list)
{
	struct list_node *cur = list->head;
	struct list_node *next = NULL;
	while (cur != NULL) {
		next = cur->next;
		free(cur);
		cur = next;
	}

	free(list);
}

void linked_list_push_back(struct linked_list *list, void *value)
{
	struct list_node *new = malloc(LIST_NODE_SIZE(list));
	if (!list->head) {
		list->head = new;
	} else {
		struct list_node *tail = list->tail;
		tail->next = new;
	}

	list->tail = new;
	memcpy(&new->value, value, list->value_size);
	list->count++;
}

void linked_list_index_remove(struct linked_list *list, unsigned int index)
{
	unsigned int cur_index = 0;
	struct list_node *cur = list->head, *prev = NULL;

	while (cur != NULL && cur_index != index) {
		if (cur_index == index) {
			linked_list_update_head_tail(list, cur, prev);
			free(cur);
			list->count--;
			
			return;
		}

		prev = cur;
		cur = cur->next;
	}
}

void linked_list_remove(struct linked_list *list, void *value)
{
	struct list_node *cur = list->head, *prev = NULL;
	
	while (cur != NULL) {
		if (list->equals(&cur->value, value)) {
			linked_list_update_head_tail(list, cur, prev);
			free(cur);
			list->count--;
			
			return;
		}
	}
}

void *linked_list_get(struct linked_list *list, void *value)
{
	struct list_node *cur = list->head;

	while (cur != NULL) {
		if (list->equals(cur->value, value)) {
			return cur->value;
		}

		cur = cur->next;
	}

	return NULL;
}

void linked_list_update_head_tail(struct linked_list *list, struct list_node *cur, struct list_node *prev)
{
	if (prev) {
		prev->next = cur->next;

		if (!cur->next) {
			list->tail = prev;
		}
	} else {
		list->head = NULL;
		list->tail = NULL;
	}
}
