#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

#define LINKED_LIST_VALUE(node) ((void*)&node->value)
#define LINKED_LIST_INITIALIZER(_value_size, _equals)			\
	(struct linked_list) {						\
		.head = NULL,						\
		.tail = NULL,					        \
		.value_size = _value_size,		         	\
		.count = 0,				            	\
		.equals = _equals                                 	\
	}					                   	

typedef int(*equals_fun)(void*, void*);

struct list_node {
	struct list_node *next;
	char value[];
};

struct linked_list {
	struct list_node *head;
	struct list_node *tail;
	size_t value_size;
	unsigned int count;
	equals_fun equals;
};

struct linked_list *linked_list_init(struct linked_list *list, size_t value_size, equals_fun equals);
void linked_list_free(struct linked_list *list);

void linked_list_push_back(struct linked_list *list, void *value);
void linked_list_index_remove(struct linked_list *list, unsigned int index);
void linked_list_remove(struct linked_list *list, void *value);
void *linked_list_get(struct linked_list *list, void *value);
void linked_list_shift_left(struct linked_list *list);

struct linked_list *linked_list_tail(struct linked_list *list);
void linked_list_update_head_tail(struct linked_list *list, struct list_node *cur, struct list_node *prev);

#endif	/* LINKED_LIST_H */

