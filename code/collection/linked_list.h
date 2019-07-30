#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

#define LINKED_LIST_VALUE(linked_list) ((void*)&linked_list->value)

typedef int(*equal_fun)(void*, void*);

struct linked_list {
	int has_value;
	struct linked_list *next;
	size_t value_size;
	equal_fun equal_fun;
	char value[];
};

struct linked_list *linked_list_init(size_t value_size);
void linked_list_free(struct linked_list *linked_list);

void linked_list_push_back(struct linked_list *linked_list, void *value);
void linked_list_index_remove(struct linked_list *linked_list, unsigned int index);
void linked_list_remove(struct linked_list *linked_list, void *value);

struct linked_list *linked_list_tail(struct linked_list *linked_list);

#endif	/* LINKED_LIST_H */
