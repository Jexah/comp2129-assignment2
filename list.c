#include "list.h"

void init_list_head(list_element *list_head_ptr)
{
	list_head_ptr->next = 0;
	list_head_ptr->prev = 0;
}

void append_element_to_list(list_element *new_element_ptr, list_element *list_ptr)
{
	while(list_ptr->next)
	{
		list_ptr = list_ptr->next;
	}
	list_ptr->next = new_element_ptr;
	new_element_ptr->prev = list_ptr;
}

void delete_list_element(list_element *element_ptr)
{
	element_ptr->prev->next = element_ptr->next;
	element_ptr->next->prev = element_ptr->prev;
	free(element_ptr);
}
