#ifndef LIST_H
#define LIST_H

struct list_element
{
	void *value;
	struct list_element *next;
	struct list_element *prev;
};

void init_list_head(struct list_element *);

void append_element_to_list(struct list_element *, struct list_element *);

void delete_list_element(struct list_element *);

#endif
