#ifndef LIST_H
#define LIST_H

struct list_element
{
	void *value;
	struct list_element *next;
	struct list_element *prev;
} list_element;

void init_list_head(list_element *);

void append_element_to_list(list_element *, list_element *);

void delete_list_element(list_element *);

#endif
