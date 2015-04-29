#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "snapshot.h"

#define TESTING 1
#if TESTING
#define DEBUG(str)(printf("%s", str))
#else
#define DEBUG(str)
#endif

// **************************************************************
// *********************   Helper module    *********************
// **************************************************************
// Notes:
//			None

// Helper function to convert a string to lowercase.
void str_tolower(char *str)
{
	while(*str)
	{
		*str = tolower(*str);
		str++;
	}
}

int compare_values(const void *_value1, const void *_value2)
{
	int *value1 = (int *)_value1;
	int *value2 = (int *)_value2;

	if(*value1 > *value2)
	{
		return 1;
	}
	if(*value1 < *value2)
	{
		return -1;
	}
	return 0;
}

// //////////////////////////////////////////////////////////////
// /////////////////////   Helper module    /////////////////////
// //////////////////////////////////////////////////////////////


// **************************************************************
// *********************   Database module  *********************
// **************************************************************

STATUS get_new_value(int number, value **new_value_ptr)
{
    *new_value_ptr = calloc(sizeof(value), 1);
	(*new_value_ptr)->value = number;
	if(!*new_value_ptr)
	{
		DEBUG("get_new_value-> !new_value_ptr\n");
		return MALLOC_FAILED;
	}
	DEBUG("get_new_value-> OK\n");
	return OK;
}

STATUS append_value_to_entry(value *new_value_ptr, entry *entry_ptr)
{
    value *cursor = entry_ptr->values;
	while(cursor->next)
	{
        cursor = cursor->next;
	}
    cursor->next = new_value_ptr;
    new_value_ptr->prev = cursor;
	DEBUG("append_value_to_entry-> OK\n");
	return OK;
}

STATUS append_int_to_entry(int number, entry *entry_ptr)
{
	value *new_value_ptr;
	STATUS get_new_value_status = get_new_value(number, &new_value_ptr);
	if(get_new_value_status != OK)
	{
		DEBUG("append_int_to_entry->get_new_value_status !OK\n");
		return get_new_value_status;
	}
    return append_value_to_entry(new_value_ptr, entry_ptr);
}

STATUS delete_value(value *target_value)
{
	if(target_value->next)
	{
		target_value->next->prev = target_value->prev;
	}
	if(target_value->prev)
	{
		target_value->prev->next = target_value->next;
	}
	free(target_value);
	return OK;
}

STATUS delete_entry_values(entry *target_entry)
{
	if(!target_entry)
	{
		DEBUG("delete_entry_values-> !target_entry\n");
		return NO_ENTRY;
	}
	if(!target_entry->values)
	{
		DEBUG("delete_entry_values->target_entry !values, OK\n");
		return OK;
	}
	value *cursor = target_entry->values->next;
	while(cursor)
	{
		value *next = cursor->next;
		delete_value(cursor);
		cursor = next;
	};
	DEBUG("delete_entry_values-> OK\n");
	return OK;
}

STATUS delete_entry(entry *target_entry)
{
	if(!target_entry)
	{
		DEBUG("delete_entry-> !target_entry, OK\n");
		return OK;
	}
	if(target_entry->next)
	{
		target_entry->next->prev = target_entry->prev;
	}
	if(target_entry->prev)
	{
		target_entry->prev->next = target_entry->next;
	}
    STATUS delete_status = delete_entry_values(target_entry);
	if(delete_status != OK)
	{
		DEBUG("delete_entry->delete_status !OK\n");
		return delete_status;
	}
	free(target_entry->values);
    free(target_entry);
	DEBUG("delete_entry-> OK\n");
	return OK;
}

STATUS free_entries_from_head(entry *entry_head)
{
	entry *cursor = entry_head->next;
	while(cursor)
	{
		entry_head = cursor;
		cursor = cursor->next;
		STATUS delete_status = delete_entry(entry_head);
		if(delete_status != OK)
		{
			DEBUG("free_entries_from_head->delete_status !OK\n");
			return delete_status;
		}
	}
	DEBUG("free_entries_from_head-> OK\n");
	return OK;
}

STATUS free_entries_and_head(entry *entry_head)
{
	if(!entry_head)
	{
		DEBUG("free_entries_and_head-> !entry_head, OK\n");
		return OK;
	}
	STATUS free_entries_status = free_entries_from_head(entry_head);
	if(free_entries_status != OK)
	{
		DEBUG("free_entries_and_head->free_entries_status !OK\n");
		return free_entries_status;
	}
	return delete_entry(entry_head);
}

STATUS append_entry_to_entry_head(entry *new_entry_ptr, entry *entry_head_ptr)
{
    entry *cursor = entry_head_ptr;
    while(cursor && cursor->next)
	{
        cursor = cursor->next;
	}
    cursor->next = new_entry_ptr;
    new_entry_ptr->prev = cursor;
	DEBUG("append_entry_to_entry_head-> OK\n");
	return OK;
}

STATUS push_entry_on_entry_head(entry *new_entry_ptr, entry *entry_head_ptr)
{
	entry *new_second = entry_head_ptr->next;
	if(new_second)
	{
		new_second->prev = new_entry_ptr;
	}
	new_entry_ptr->prev = entry_head_ptr;
	new_entry_ptr->next = new_second;
	entry_head_ptr->next = new_entry_ptr;
	DEBUG("push_entry_on_entry_head-> OK\n");
	return OK;
}

entry *find_entry_by_key(char *key_ptr, entry *entry_head)
{
	entry *cursor_ptr = entry_head;
	while(cursor_ptr)
	{
		if(strcmp(cursor_ptr->key, key_ptr) == 0)
		{
			return cursor_ptr;
		}
		cursor_ptr = cursor_ptr->next;
	}
	return 0;
}

STATUS set_entry_values_by_key(char *key, char *values, entry *entry_head)
{
	STATUS method_status = delete_entry_values_by_key(key, entry_head);
	if(method_status != OK && method_status != NO_KEY)
	{
		DEBUG("set_entry_values_by_key->delete_entry_values_by_key !OK !NO_ENTRY\n");
		return method_status;
	}
	method_status = create_entry_if_not_exist(key, entry_head);
	if(method_status != OK)
	{
		DEBUG("set_entry_values_by_key->create_entry_if_not_exist !OK !NO_ENTRY\n");
		return method_status;
	}
	method_status = append_entry_values_by_key(key, values, entry_head);
	if(method_status != OK)
	{
		DEBUG("set_entry_values_by_key->append_entry_values_by_key !OK\n");
		return method_status;
	}
	DEBUG("set_entry_values_by_key OK\n");
	return OK;
}

void print_values_in_entry(entry *entry_head)
{
	if(!entry_head)
	{
		DEBUG("print_values_in_entry-> !entry_head\n");
		printf("no such entry\n");

	}
	value *value_cursor = entry_head->values->next;
	printf("[");
	while(value_cursor)
	{
		print_value_by_value(value_cursor);
		value_cursor = value_cursor->next;
		if(value_cursor)
		{
			printf(" ");
		}
	}
	printf("]\n");
}

STATUS print_value_by_value(value *target_value)
{
	printf("%d", target_value->value);
	return OK;
}

STATUS append_entry_values_by_key(char *key, char *values, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		DEBUG("append_entry_values_by_key-> !entry_ptr, NO_KEY\n");
		return NO_KEY;
	}
	while(values)
	{
		STATUS append_int_status = append_int_to_entry(atoi(values), entry_ptr);
		if(append_int_status != OK)
		{
			DEBUG("append_entry_values_by_key->append_int_status !OK\n");
			return append_int_status;
		}
		char *next_space = strchr(values+1, ' ');
		values = (next_space ? next_space : strchr(values+1, '\n'));
	};
	DEBUG("append_entry_values_by_key-> OK\n");
	return OK;
}

STATUS get_new_entry(char *key, entry **new_entry_ptr)
{
	*new_entry_ptr = calloc(sizeof(entry), 1);
	value *entry_ptr_values_head = calloc(sizeof(value), 1);
	(*new_entry_ptr)->values = entry_ptr_values_head;
	strncpy((*new_entry_ptr)->key, key, MAX_KEY_LENGTH);
	return OK;
}

STATUS append_key_to_entry_head(char *key, entry **new_entry_ptr)
{
	STATUS get_new_entry_status = get_new_entry(key, new_entry_ptr);
	if(get_new_entry_status != OK)
	{
		DEBUG("append_key_ton_entry_head->get_new_entry_status !OK\n");
		return get_new_entry_status;
	}
	DEBUG("append_key_ton_entry_head-> OK\n");
	return OK;
}

STATUS create_entry_if_not_exist(char *key, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		STATUS get_new_entry_status = get_new_entry(key, &entry_ptr);
		if(get_new_entry_status != OK)
		{
			DEBUG("create_entry_if_not_exist->get_new_entry_status !OK\n");
			return get_new_entry_status;
		}
		STATUS append_entry_status = push_entry_on_entry_head(entry_ptr, entry_head);
		if(append_entry_status != OK)
		{
			DEBUG("create_entry_if_not_exist->append_entry_status !OK\n");
			return append_entry_status;
		}
	}
	DEBUG("create_entry_if_not_exist-> OK\n");
	return OK;
}

STATUS delete_entry_values_by_key(char *key, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		DEBUG("delete_entry_values_by_key-> !entry_ptr, NO_KEY\n");
		return NO_KEY;
	}
	STATUS delete_entry_values_status = delete_entry_values(entry_ptr);
	if(delete_entry_values_status != OK)
	{
		DEBUG("delete_entry_values_by_key->delete_entry_values_status !OK\n");
		return delete_entry_values_status;
	}
		DEBUG("delete_entry_values_by_key-> OK\n");
	return OK;
}

STATUS delete_entry_by_key(char *key, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		DEBUG("delete_entry_by_key-> !entry_ptr NO_KEY\n");
		return NO_KEY;
	}
	STATUS delete_entry_status = delete_entry(entry_ptr);
	if(delete_entry_status != OK)
	{
		DEBUG("delete_entry_by_key->delete_entry_status !OK\n");
		return delete_entry_status;
	}
		DEBUG("delete_entry_by_key-> OK\n");
	return OK;
}

STATUS purge_entry(char *key, entry *entry_head, snapshot *snapshot_head)
{
	STATUS del_status = delete_entry_by_key(key, entry_head);
	if(del_status != OK && del_status != NO_KEY)
	{
		DEBUG("purge_entry->del_status [current] !OK !NO_KEY\n");
		return del_status;
	}
	snapshot *snapshot_cursor = snapshot_head->next;
	while(snapshot_cursor)
	{
		del_status = delete_entry_by_key(key, snapshot_cursor->entries);
		if(del_status != OK && del_status != NO_KEY)
		{
			DEBUG("purge_entry->del_status [snapshot] !OK !NO_KEY\n");
			return del_status;
		}
		snapshot_cursor = snapshot_cursor->next;
	}
	DEBUG("purge_entry-> OK\n");
	return OK;
}

STATUS push_int_on_entry(int number, entry *target_entry)
{
	value *new_value;
	STATUS get_new_value_status = get_new_value(number, &new_value);
	if(get_new_value_status != OK)
	{
		DEBUG("push_int_on_entry->get_new_value_status !OK\n");
		return get_new_value_status;
	}
	return push_value_on_entry(new_value, target_entry);
}

STATUS push_value_on_entry(value *new_value, entry *target_entry)
{
	value *new_second = target_entry->values->next;
	target_entry->values->next = new_value;
	new_value->next = new_second;
	if(new_second)
	{
		new_second->prev = new_value;
	}
	DEBUG("push_value_on_entry-> OK\n");
	return OK;
}

STATUS push_ints_on_entry_by_key(char *key, char *values, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		DEBUG("push_int_on_entry_by_key-> !entry_ptr, NO_KEY\n");
		return NO_KEY;
	}
	while(values)
	{
		STATUS push_int_status = push_int_on_entry(atoi(values), entry_ptr);
		if(push_int_status != OK)
		{
			DEBUG("push_int_on_entry_by_key->push_int_status !OK\n");
			return push_int_status;
		}
		char *next_space = strchr(values+1, ' ');
		values = (next_space ? next_space : strchr(values+1, '\n'));
	};
	DEBUG("push_int_on_entry_by_key-> OK\n");
	return OK;
}

value *get_value_from_entry_by_index(int index, entry *target_entry)
{
	value *cursor = target_entry->values;
	for(int i = 0; i < index; ++i)
	{
		if(!cursor->next)
		{
			DEBUG("get_value_from_entry_by_index-> !cursor->next, INDEX_OUT_OF_RANGE\n");
			return 0;
		}
		cursor = cursor->next;
	}
	return cursor;
}

STATUS print_value_index_by_entry(int index, entry *target_entry)
{
	value *target_value = get_value_from_entry_by_index(index, target_entry);
	if(!target_value)
	{
		DEBUG("print_value_index_by_entry-> !target_value, INDEX_OUT_OF_RANGE\n");
		return INDEX_OUT_OF_RANGE;
	}
	DEBUG("print_value_index_by_entry-> OK\n");
	printf("%d", target_value->value);
	return OK;
}

STATUS print_value_index_by_key(int index, char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("print_value_index_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS print_value_status = print_value_index_by_entry(index, found);
	if(print_value_status != OK)
	{
		return print_value_status;
	}
	DEBUG("print_value_index_by_key-> OK\n");
	return OK;
}

STATUS print_and_remove_index_by_entry(int index, entry *target_entry)
{
	value *target_value = get_value_from_entry_by_index(index, target_entry);
	if(!target_value)
	{
		DEBUG("print_and_remove_index_by_entry-> !target_value, INDEX_OUT_OF_RANGE\n");
		return INDEX_OUT_OF_RANGE;
	}
	STATUS print_value_status = print_value_by_value(target_value);
	if(print_value_status != OK)
	{
		DEBUG("print_and_remove_index_by_entry->print_value_status !OK\n");
		return print_value_status;
	}
	STATUS delete_value_status = delete_value(target_value);
	if(delete_value_status != OK)
	{
		DEBUG("print_and_remove_index_by_entry->delete_value_status !OK\n");
		return delete_value_status;
	}
	DEBUG("print_and_remove_index_by_entry-> OK\n");
	return OK;
}

STATUS print_and_remove_index_by_key(int index, char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("print_value_index_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS print_and_remove_index_status = print_and_remove_index_by_entry(index, found);
	if(print_and_remove_index_status != OK)
	{
		DEBUG("print_value_index_by_key->print_and_remove_index !OK\n");
		return print_and_remove_index_status;
	}
	return OK;
}

STATUS delete_value_index_by_entry(int index, entry *target_entry)
{
	value *found = get_value_from_entry_by_index(index, target_entry);
	if(!found)
	{
		DEBUG("delete_value_index_by_entry-> !found, INDEX_OUT_OF_RANGE\n");
		return INDEX_OUT_OF_RANGE;
	}
	STATUS delete_status = delete_value(found);
	if(delete_status != OK)
	{
		DEBUG("delete_value_index_by_entry->delete_status !OK\n");
		return delete_status;
	}
	return OK;
}

STATUS delete_value_index_by_key(int index, char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("delete_value_index_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS delete_status = delete_value_index_by_entry(index, found);
	if(delete_status != OK)
	{
		DEBUG("delete_value_index_by_entry->delete_status !OK\n");
		return delete_status;
	}
	return OK;
}

STATUS print_minimum_value_by_entry(entry *target_entry)
{
	int lowest;
	int first = 1;
	value *cursor = target_entry->values->next;
	while(cursor)
	{
		if(cursor->value < lowest || first == 1)
		{
			lowest = cursor->value;
		}
		cursor = cursor->next;
		first = 0;
	}
	if(first)
	{
		DEBUG("print_minimum_value_by_entry-> first\n");
		return INDEX_OUT_OF_RANGE;
	}
	printf("%d", lowest);
	DEBUG("print_minimum_value_by_entry-> OK\n");
	return OK;
}

STATUS print_minimum_value_by_key(char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("print_minimum_value_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS print_min_value_status = print_minimum_value_by_entry(found);
	if(print_min_value_status != OK)
	{
		DEBUG("print_minimum_value_by_key->print_min_value_status !OK\n");
		return print_min_value_status;
	}
	return OK;
}

STATUS print_maximum_value_by_entry(entry *target_entry)
{
	int highest;
	int first = 1;
	value *cursor = target_entry->values->next;
	while(cursor)
	{
		if(cursor->value > highest || first == 1)
		{
			highest = cursor->value;
		}
		cursor = cursor->next;
		first = 0;
	}
	if(first)
	{
		DEBUG("print_maximum_value_by_entry-> first\n");
		return INDEX_OUT_OF_RANGE;
	}
	printf("%d", highest);
	DEBUG("print_maximum_value_by_entry-> OK\n");
	return OK;
}

STATUS print_maximum_value_by_key(char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("print_maximum_value_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS print_max_value_status = print_maximum_value_by_entry(found);
	if(print_max_value_status != OK)
	{
		DEBUG("print_maximum_value_by_key->print_max_value_status !OK\n");
		return print_max_value_status;
	}
	return OK;
}

STATUS print_sum_of_values_by_entry(entry *target_entry)
{
	int total = 0;
	value *cursor = target_entry->values->next;
	while(cursor)
	{
		total += cursor->value;
		cursor = cursor->next;
	}
	printf("%d", total);
	DEBUG("print_sum_of_values_by_entry-> OK\n");
	return OK;
}

STATUS print_sum_of_values_by_key(char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("print_sum_of_values_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS print_sum_status = print_sum_of_values_by_entry(found);
	if(print_sum_status != OK)
	{
		DEBUG("print_sum_of_values_by_key->print_sum_status !OK\n");
		return print_sum_status;
	}
	return OK;
}

int get_len_of_values_by_entry(entry *target_entry)
{
	int total = 0;
	value *cursor = target_entry->values->next;
	while(cursor)
	{
		total++;
		cursor = cursor->next;
	}
	return total;
}

STATUS print_len_of_values_by_entry(entry *target_entry)
{
	printf("%d", get_len_of_values_by_entry(target_entry));
	DEBUG("print_len_of_values_by_entry-> OK\n");
	return OK;
}

STATUS print_len_of_values_by_key(char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("print_len_of_values_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS print_len_status = print_len_of_values_by_entry(found);
	if(print_len_status != OK)
	{
		DEBUG("print_len_of_values_by_key->print_sum_status !OK\n");
		return print_len_status;
	}
	return OK;
}

STATUS rev_values_by_entry(entry *target_entry)
{
	int total = 0;
	value *cursor = target_entry->values->next;
	while(cursor)
	{
		value *next = cursor->next;
		value *prev = cursor->prev;
		cursor->next = cursor->prev;
		cursor->prev = next;

		if(prev == target_entry->values)
		{
			cursor->next = 0;
		}
		if(!next)
		{
			target_entry->values->next = cursor;
			cursor->prev = target_entry->values;
		}
		cursor = next;
	}
	DEBUG("rev_values_by_entry-> OK\n");
	return OK;
}

STATUS rev_values_by_key(char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("rev_values_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS rev_status = rev_values_by_entry(found);
	if(rev_status != OK)
	{
		DEBUG("rev_values_by_key->rev_status !OK\n");
		return rev_status;
	}
	return OK;
}

STATUS uniq_values_by_entry(entry *target_entry)
{
	int prev_value;
	int first = 1;
	value *cursor = target_entry->values->next;
	while(cursor)
	{
		value *next = cursor->next;
		if(prev_value == cursor->value && !first)
		{
			delete_value(cursor);
		}
		prev_value = cursor->value;
		cursor = next;
		first = 0;
	}
	DEBUG("uniq_values_by_entry-> OK\n");
	return OK;
}

STATUS uniq_values_by_key(char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("uniq_values_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS uniq_status = uniq_values_by_entry(found);
	if(uniq_status != OK)
	{
		DEBUG("uniq_values_by_key->uniq_status !OK\n");
		return uniq_status;
	}
	return OK;
}

STATUS sort_values_by_entry(entry *target_entry)
{
	int count = get_len_of_values_by_entry(target_entry);
	int arr[count];
	int current = 0;
	value *cursor = target_entry->values->next;
	while(cursor)
	{
		arr[current] = cursor->value;
		current++;
		cursor = cursor->next;
	}
	qsort(arr, count, sizeof(int), compare_values);
	current = 0;
	cursor = target_entry->values->next;
	while(cursor)
	{
		cursor->value = arr[current];
		current++;
		cursor = cursor->next;
	}
	DEBUG("sort_values_by_entry-> OK\n");
	return OK;
}

STATUS sort_values_by_key(char *key, entry *entry_head)
{
	entry *found = find_entry_by_key(key, entry_head);
	if(!found)
	{
		DEBUG("sort_values_by_key-> !found\n");
		return NO_KEY;
	}
	STATUS sort_status = sort_values_by_entry(found);
	if(sort_status != OK)
	{
		DEBUG("sort_values_by_key->sort_status !OK\n");
		return sort_status;
	}
	return OK;
}

STATUS push_entry_on_snapshot(entry *new_entry, snapshot *target_snapshot)
{
	entry *new_second = target_snapshot->entries->next;
	target_snapshot->entries->next = new_entry;
	new_entry->next = new_second;
	new_entry->prev = target_snapshot->entries;
	if(new_second)
	{
		new_second->prev = new_entry;
	}
	DEBUG("push_entry_on_snapshot-> OK\n");
	return OK;
}

STATUS push_key_on_snapshot(char *key, snapshot *target_snapshot)
{
	entry *new_entry;
	STATUS get_new_entry_status = get_new_entry(key, &new_entry);
	if(get_new_entry_status != OK)
	{
		DEBUG("push_key_on_snapshot->get_new_entry_status !OK\n");
		return get_new_entry_status;
	}
	return push_entry_on_snapshot(new_entry, target_snapshot);
}

STATUS take_snapshot(snapshot *snapshot_head, entry *entry_head, int *latest_snapshotID)
{
	snapshot *new_snapshot = calloc(sizeof(snapshot), 1);
	new_snapshot->entries = calloc(sizeof(entry), 1);
	new_snapshot->id = (*latest_snapshotID)++;
	snapshot *snapshot_new_second = snapshot_head->next;
	snapshot_head->next = new_snapshot;
	new_snapshot->prev = snapshot_head;
	new_snapshot->next = snapshot_new_second;

	if(snapshot_new_second)
	{
		snapshot_new_second->prev = new_snapshot;
	}

	entry *entry_cursor = entry_head->next;
	while(entry_cursor)
	{
		STATUS get_new_entry_status = push_key_on_snapshot(entry_cursor->key, new_snapshot);
		if(get_new_entry_status != OK)
		{
			DEBUG("take_snapshot->get_new_entry_status !OK\n");
			return get_new_entry_status;
		}
		value *value_cursor = entry_cursor->values->next;
		while(value_cursor)
		{
			entry *current_entry_in_snapshot = find_entry_by_key(entry_cursor->key, new_snapshot->entries);
			STATUS get_new_value_status = append_int_to_entry(value_cursor->value, current_entry_in_snapshot);
			if(get_new_value_status != OK)
			{
				DEBUG("take_snapshot->get_new_value_status !OK\n");
				return get_new_value_status;
			}
			value_cursor = value_cursor->next;
		}
		entry_cursor = entry_cursor->next;
	}
	DEBUG("take_snapshot-> OK\n");
	return OK;
}

snapshot *find_snapshot_by_id(int id, snapshot *snapshot_head)
{
	snapshot *cursor = snapshot_head->next;
	while(cursor)
	{
		if(cursor->id == id)
		{
			return cursor;
		}
		cursor = cursor->next;
	}
	return 0;
}

STATUS delete_snapshot_by_snapshot(snapshot *target_snapshot, snapshot *snapshot_head)
{
	if(target_snapshot->next) target_snapshot->next->prev = target_snapshot->prev;
	if(target_snapshot->prev) target_snapshot->prev->next = target_snapshot->next;

	STATUS free_entries_status = free_entries_and_head(target_snapshot->entries);
	if(free_entries_status != OK)
	{
		DEBUG("delete_snapshot_by_snapshot->free_entries_status !OK\n");
		return free_entries_status;
	}
	free(target_snapshot);
	return OK;
}

STATUS delete_snapshot_by_id(int id, snapshot *snapshot_head)
{
	snapshot *found = find_snapshot_by_id(id, snapshot_head);
	if(!found)
	{
		DEBUG("delete_snapshot_by_id-> !found, NO_SNAPSHOT");
		return NO_SNAPSHOT;
	}
	return delete_snapshot_by_snapshot(found, snapshot_head);
}

STATUS delete_all_snapshots(snapshot *snapshot_head)
{
	snapshot *cursor = snapshot_head->next;
	while(cursor)
	{
		snapshot *next = cursor->next;
		STATUS delete_snapshot_status = delete_snapshot_by_snapshot(cursor, snapshot_head);
		if(delete_snapshot_status != OK)
		{
			DEBUG("delete_all_snapshots->delete_snapshot_status !OK");
			return delete_snapshot_status;
		}
		DEBUG("delete_all_snapshots->delete_snapshot_status OK");
		cursor = next;
	}
	return delete_snapshot_by_snapshot(snapshot_head, snapshot_head);
}

STATUS free_all(snapshot *snapshot_head, entry *entry_head)
{
	STATUS delete_snapshots_status = delete_all_snapshots(snapshot_head);
	if(delete_snapshots_status != OK)
	{
		DEBUG("free_all->delete_snapshots_status !OK");
		return delete_snapshots_status;
	}
	DEBUG("free_all->delete_snapshots_status OK");
	return free_entries_and_head(entry_head);
}

STATUS restore_snapshot_by_snapshot(snapshot *target_snapshot, entry *entry_head)
{
	STATUS free_entries_status = free_entries_from_head(entry_head);
	if(free_entries_status != OK)
	{
		DEBUG("restore_snapshot_by_snapshot->free_entries_status !OK");
		return free_entries_status;
	}
	entry *snapshot_entry_cursor = target_snapshot->entries->next;
	entry *entry_head_cursor = entry_head->next;
	while(snapshot_entry_cursor)
	{
		STATUS get_new_entry_status = get_new_entry(snapshot_entry_cursor->key, &entry_head_cursor);
		if(get_new_entry_status != OK)
		{
			DEBUG("restore_snapshot_by_snapshot->get_new_entry_status !OK");
			return get_new_entry_status;
		}
		STATUS append_entry_status = append_entry_to_entry_head(entry_head_cursor, entry_head);
		if(append_entry_status != OK)
		{
			DEBUG("restore_snapshot_by_snapshot->append_entry_status !OK");
			return append_entry_status;
		}
		value *snapshot_value_cursor = snapshot_entry_cursor->values->next;
		value *value_head_cursor = entry_head_cursor->values->next;
		while(snapshot_value_cursor)
		{
			STATUS get_new_value_status = get_new_value(snapshot_value_cursor->value, &value_head_cursor);
			if(get_new_value_status != OK)
			{
				DEBUG("restore_snapshot_by_snapshot->get_new_value_status !OK");
				return get_new_value_status;
			}
			STATUS append_value_status = append_value_to_entry(value_head_cursor, entry_head_cursor);
			if(append_value_status != OK)
			{
				DEBUG("restore_snapshot_by_snapshot->append_value_status !OK");
				return append_value_status;
			}
			snapshot_value_cursor = snapshot_value_cursor->next;
		}
		snapshot_entry_cursor = snapshot_entry_cursor->next;
		entry_head_cursor = entry_head_cursor->next;
	}
	return OK;
}

STATUS restore_snapshot_by_id(int id, snapshot *snapshot_head, entry *entry_head)
{
	snapshot *found = find_snapshot_by_id(id, snapshot_head);
	if(!found)
	{
		DEBUG("restore_snapshot_by_id-> !found, NO_SNAPSHOT");
		return NO_SNAPSHOT;
	}
	return restore_snapshot_by_snapshot(found, entry_head);
}

// //////////////////////////////////////////////////////////////
// ////////////////////   Database module    ////////////////////
// //////////////////////////////////////////////////////////////



// **************************************************************
// *********************   Command module   *********************
// **************************************************************
// Notes:
//			Short argument is defined as an argument that does not contain spaces.
//			Long argument is defined as an argument that also contains spaces.

// Take input string and return the first short argument.
char *get_arg_from_pointer_malloc_ptr(char *start_ptr)
{
	char *first_space = strchr(start_ptr, ' ');
	char *return_ptr = calloc(((first_space ? first_space : strchr(start_ptr, '\n')) - start_ptr) + 1 , 1);
	char *rolling_ptr = return_ptr;
	while(*start_ptr != ' ' && *start_ptr != '\n' && (*rolling_ptr++ = *start_ptr++));
	return return_ptr;
}

// Take input string and return the first long argument found.
char *get_long_arg_from_pointer_malloc_ptr(char *start_ptr)
{
	char *return_malloc_ptr = calloc((strchr(start_ptr, '\n') - start_ptr) + 1, 1);
	char *rolling_ptr = return_malloc_ptr;
	while(*start_ptr != '\n' && (*rolling_ptr++ = *start_ptr++));
	return return_malloc_ptr;
}

// Returns a (char *) to the argument index specified.
char *get_pointer_to_arg_ptr(char *start_ptr, int arg)
{
	for(int i = 0; i < arg; ++i)
	{
		start_ptr = strchr(start_ptr, ' ') + 1;
	}
	return start_ptr;
}

// Returns a pointer to the specified short argument in the specified input. The pointer must be freed.
char *get_arg_malloc_ptr(char *input_ptr, int arg)
{
	return get_arg_from_pointer_malloc_ptr(get_pointer_to_arg_ptr(input_ptr, arg));
}

// Returns a pointer to the specified long argument in the specified input. The pointer must be freed.
char *get_long_arg_malloc_ptr(char *input_ptr, int arg)
{
	return get_long_arg_from_pointer_malloc_ptr(get_pointer_to_arg_ptr(input_ptr, arg));
}

// Returns the number of short arguments in the string.
int get_num_args(char *start_ptr)
{
	int num_args = (!!*start_ptr);
	while(*start_ptr++)
	{
		num_args += *start_ptr == ' ';
	}
	return num_args;
}

// Returns a command object based on given input.
command_struct *get_command_struct(char *input_ptr)
{
	if(!*input_ptr) return 0;
	command_struct *malloc_ptr = calloc(sizeof(command_struct), 1);
	int num_args = get_num_args(input_ptr);
	if(num_args >= 1) malloc_ptr->args_malloc_ptr[0] = get_arg_malloc_ptr(input_ptr, 0);
	str_tolower(malloc_ptr->args_malloc_ptr[0]);
	if(num_args >= 2) malloc_ptr->args_malloc_ptr[1] = get_arg_malloc_ptr(input_ptr, 1);
	if(num_args >= 3) malloc_ptr->args_malloc_ptr[2] = get_long_arg_malloc_ptr(input_ptr, 2);
	return malloc_ptr;
}

void free_command(command_struct *command)
{
	for(int i = 0; i < 3; ++i)
	{
		free(command->args_malloc_ptr[i]);
	}
	free(command);
}

// //////////////////////////////////////////////////////////////
// /////////////////////   Command module   /////////////////////
// //////////////////////////////////////////////////////////////




// **************************************************************
// *********************   Options module   *********************
// **************************************************************
// Notes:
//			None

// Prints help string
void print_help_string(void)
{
	printf(\
		"BYE clear database and exit\n"\
		"HELP display this help message\n"\
		"\n"\
		"LIST KEYS displays all keys in current state\n"\
		"LIST ENTRIES displays all entries in current state\n"\
		"LIST SNAPSHOTS displays all snapshot ids\n"\
		"\n"\
		"GET <key> displays entry values\n"\
		"DEL <key> deletes entry from current state\n"\
		"PURGE <key> deletes entry from current state and snapshots\n"\
		"\n"\
		"SET <key> <value ...> sets entry values\n"\
		"PUSH <key> <value ...> pushes each value to the front one at a time\n"\
		"APPEND <key> <value ...> append each value to the back one at a time\n"\
		"\n"\
		"PICK <key> <index> displays entry value at index\n"\
		"PLUCK <key> <index> displays and removes entry value at index\n"\
		"POP <key> displays and removes the front entry value\n"\
		"\n"\
		"DROP <id> deletes snapshot\n"\
		"ROLLBACK <id> restores to snapshot and deletes newer snapshots\n"\
		"CHECKOUT <id> replaces current state with a copy of snapshot\n"\
		"SNAPSHOT saves the current state as a snapshot\n"\
		"\n"\
		"MIN <key> displays minimum entry value\n"\
		"MAX <key> displays maximum entry value\n"\
		"SUM <key> displays sum of entry values\n"\
		"LEN <key> displays number of entry values\n"\
		"\n"\
		"REV <key> reverses order of entry values\n"\
		"UNIQ <key> removes repeated adjacent entry values\n"\
		"SORT <key> sorts entry values in ascending order\n"
	);
}

void get_command(command_struct *command, entry *entry_head)
{
	entry *found = find_entry_by_key(command->args_malloc_ptr[1], entry_head);
	if(!found)
	{
		printf("no such key\n");
		return;
	}
	print_values_in_entry(found);
}

void del_command(command_struct *command, entry *entry_head)
{
	STATUS status = delete_entry_by_key(command->args_malloc_ptr[1], entry_head);
	switch(status)
	{
		case OK:
			printf("ok\n");
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		default:
			printf("unknown error (del command): %d", status);
			break;
	}
}

void purge_command(command_struct *command, entry *entry_head, snapshot *snapshot_head)
{
	STATUS status = purge_entry(command->args_malloc_ptr[1], entry_head, snapshot_head);
	switch(status)
	{
		case OK:
			printf("ok\n");
			break;
		default:
			printf("whoops (purge command: %d)", status);
			break;
	}
}

STATUS list_command_keys(entry *entry_head)
{
	if(entry_head->next)
	{
		entry *cursor = entry_head->next;
		while(cursor)
		{
			printf("%s\n", cursor->key);
			cursor = cursor->next;
		}
	}
	else
	{
		return NO_KEYS;
	}
	return OK;
}

STATUS list_command_entries(entry *entry_head)
{
	if(entry_head->next)
	{
		entry *entry_cursor = entry_head->next;
		while(entry_cursor)
		{
			printf("%s [", entry_cursor->key);
			value *value_cursor = entry_cursor->values->next;
			while(value_cursor)
			{
				printf("%d", value_cursor->value);
				value_cursor = value_cursor->next;
				if(value_cursor)
				{
					printf(" ");
				}
			}
			entry_cursor = entry_cursor->next;
			printf("]\n");
		}
	}
	else
	{
		return NO_ENTRIES;
	}
	return OK;
}

STATUS list_command_snapshots(snapshot *snapshot_head)
{
	if(snapshot_head->next)
	{
		snapshot *cursor = snapshot_head->next;
		while(cursor)
		{
			printf("%d\n", cursor->id);
			cursor = cursor->next;
		}
	}
	else
	{
		return NO_SNAPSHOTS;
	}
	return OK;
}

void list_command(command_struct *command, entry *entry_head, snapshot *snapshot_head)
{
	str_tolower(command->args_malloc_ptr[1]);
	if(strcmp(command->args_malloc_ptr[1], "keys") == 0)
	{
		STATUS list_keys_status = list_command_keys(entry_head);
		switch(list_keys_status)
		{
			case OK:
				return;
			case NO_KEYS:
				printf("no keys\n");
				return;
			default:
				printf("Whoops! (list_command [keys]: %d)", list_keys_status);
				return;
		}
	}
	else if(strcmp(command->args_malloc_ptr[1], "entries") == 0)
	{
		STATUS list_entries_status = list_command_entries(entry_head);
		switch(list_entries_status)
		{
			case OK:
				return;
			case NO_ENTRIES:
				printf("no entries\n");
				return;
			default:
				printf("Whoops! (list_command [entries]: %d)", list_entries_status);
				return;
		}
	}
	else if(strcmp(command->args_malloc_ptr[1], "snapshots") == 0)
	{
		STATUS list_snapshots_status = list_command_snapshots(snapshot_head);
		switch(list_snapshots_status)
		{
			case OK:
				return;
			case NO_SNAPSHOTS:
				printf("no snapshots\n");
				return;
			default:
				printf("Whoops! (list_command [snapshots]: %d)", list_snapshots_status);
				return;
		}
	}
}

void set_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1] || !command->args_malloc_ptr[2])
	{
		printf("invalid input\n");
		return;
	}
	STATUS set_entry_values_status = set_entry_values_by_key(command->args_malloc_ptr[1], command->args_malloc_ptr[2], entry_head);
	switch(set_entry_values_status)
	{
		case OK:
			printf("ok\n");
			break;
		default:
			printf("Whoops! (set_command: %d)", set_entry_values_status);
			break;
	}
}

void push_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1] || !command->args_malloc_ptr[2])
	{
		printf("invalid input\n");
		return;
	}
	STATUS push_ints_status = push_ints_on_entry_by_key(command->args_malloc_ptr[1], command->args_malloc_ptr[2], entry_head);
	switch(push_ints_status)
	{
		case OK:
			printf("ok\n");
			break;
		default:
			printf("Whoops! (push_command: %d)", push_ints_status);
			break;
	}
}

void append_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1] || !command->args_malloc_ptr[2])
	{
		printf("invalid input\n");
		return;
	}
	STATUS append_ints_status = append_entry_values_by_key(command->args_malloc_ptr[1], command->args_malloc_ptr[2], entry_head);
	switch(append_ints_status)
	{
		case OK:
			printf("ok\n");
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		default:
			printf("Whoops! (append_command: %d)", append_ints_status);
			break;
	}
}

void pick_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1] || !command->args_malloc_ptr[2])
	{
		printf("invalid input\n");
		return;
	}
	STATUS print_value_status = print_value_index_by_key(atoi(command->args_malloc_ptr[2]), command->args_malloc_ptr[1], entry_head);
	switch(print_value_status)
	{
		case OK:
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		case INDEX_OUT_OF_RANGE:
			printf("index out of range\n");
			break;
		default:
			printf("Whoops! (pick_command: %d)\n", print_value_status);
			break;
	}
}

void pluck_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1] || !command->args_malloc_ptr[2])
	{
		printf("invalid input\n");
		return;
	}
	STATUS print_and_delete_value_status = print_and_remove_index_by_key(atoi(command->args_malloc_ptr[2]), command->args_malloc_ptr[1], entry_head);
	switch(print_and_delete_value_status)
	{
		case OK:
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		case INDEX_OUT_OF_RANGE:
			printf("index out of range\n");
			break;
		default:
			printf("Whoops! (pluck_command: %d)\n", print_and_delete_value_status);
			break;
	}
}

void pop_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1])
	{
		printf("invalid input\n");
		return;
	}
	STATUS print_and_delete_value_status = print_and_remove_index_by_key(1, command->args_malloc_ptr[1], entry_head);
	switch(print_and_delete_value_status)
	{
		case OK:
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		case INDEX_OUT_OF_RANGE:
			printf("nil\n");
			break;
		default:
			printf("Whoops! (pop_command: %d)\n", print_and_delete_value_status);
			break;
	}
}

void min_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1])
	{
		printf("invalid input\n");
		return;
	}
	STATUS print_min_value_status = print_minimum_value_by_key(command->args_malloc_ptr[1], entry_head);
	switch(print_min_value_status)
	{
		case OK:
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		case INDEX_OUT_OF_RANGE:
			printf("index out of range\n");
			break;
		default:
			printf("Whoops! (min_command: %d)", print_min_value_status);
			break;
	}
}

void max_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1])
	{
		printf("invalid input\n");
		return;
	}
	STATUS print_max_value_status = print_maximum_value_by_key(command->args_malloc_ptr[1], entry_head);
	switch(print_max_value_status)
	{
		case OK:
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		case INDEX_OUT_OF_RANGE:
			printf("index out of range\n");
			break;
		default:
			printf("Whoops! (max_command: %d)", print_max_value_status);
			break;
	}
}

void sum_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1])
	{
		printf("invalid input\n");
		return;
	}
	STATUS print_sum_status = print_sum_of_values_by_key(command->args_malloc_ptr[1], entry_head);
	switch(print_sum_status)
	{
		case OK:
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		default:
			printf("Whoops! (sum_command: %d)", print_sum_status);
			break;
	}
}

void len_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1])
	{
		printf("invalid input\n");
		return;
	}
	STATUS print_len_status = print_len_of_values_by_key(command->args_malloc_ptr[1], entry_head);
	switch(print_len_status)
	{
		case OK:
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		default:
			printf("Whoops! (len_command: %d)", print_len_status);
			break;
	}
}

void rev_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1])
	{
		printf("invalid input\n");
		return;
	}
	STATUS rev_status = rev_values_by_key(command->args_malloc_ptr[1], entry_head);
	switch(rev_status)
	{
		case OK:
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		default:
			printf("Whoops! (rev_command: %d)", rev_status);
			break;
	}
}

void uniq_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1])
	{
		printf("invalid input\n");
		return;
	}
	STATUS uniq_status = uniq_values_by_key(command->args_malloc_ptr[1], entry_head);
	switch(uniq_status)
	{
		case OK:
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		default:
			printf("Whoops! (uniq_command: %d)", uniq_status);
			break;
	}
}

void sort_command(command_struct *command, entry *entry_head)
{
	if(!command->args_malloc_ptr[1])
	{
		printf("invalid input\n");
		return;
	}
	STATUS sort_status = sort_values_by_key(command->args_malloc_ptr[1], entry_head);
	switch(sort_status)
	{
		case OK:
			printf("ok\n");
			break;
		case NO_KEY:
			printf("no such key\n");
			break;
		default:
			printf("Whoops! (sort_command: %d)", sort_status);
			break;
	}
}

void snapshot_command(snapshot *snapshot_head, entry *entry_head, int *latest_snapshotID)
{
	STATUS snapshot_status = take_snapshot(snapshot_head, entry_head, latest_snapshotID);
	switch(snapshot_status)
	{
		case OK:
			printf("ok\n");
			break;
		default:
			printf("Whoops! (snapshot_command: %d)", snapshot_status);
			break;
	}
}

void drop_command(command_struct *command, snapshot *snapshot_head)
{
	STATUS delete_snapshot_status = delete_snapshot_by_id(atoi(command->args_malloc_ptr[1]), snapshot_head);
	switch(delete_snapshot_status)
	{
		case OK:
			printf("ok\n");
			break;
		case NO_SNAPSHOT:
			printf("no such snapshot\n");
			break;
		default:
			printf("Whoops! (delete_snapshot_status: %d)", delete_snapshot_status);
			break;
	}
}

void bye_command(snapshot *snapshot_head, entry *entry_head)
{
	free_all(snapshot_head, entry_head);
}

void checkout_command(command_struct *command, snapshot *snapshot_head, entry *entry_head)
{
	STATUS checkout_snapshot_status = restore_snapshot_by_id(atoi(command->args_malloc_ptr[1]), snapshot_head, entry_head);
	switch(checkout_snapshot_status)
	{
		case OK:
			printf("ok\n");
			break;
		case NO_SNAPSHOT:
			printf("no such snapshot\n");
			break;
		default:
			printf("Whoops! (checkout_snapshot_status: %d)", checkout_snapshot_status);
			break;
	}
}

// //////////////////////////////////////////////////////////////
// /////////////////////   Options module   /////////////////////
// //////////////////////////////////////////////////////////////


int main(void)
{
	entry* entry_head = calloc(sizeof(entry), 1);
	snapshot* snapshot_head = calloc(sizeof(snapshot), 1);

	int latest_snapshotID = 1;

	char buffer[MAX_LINE_LENGTH];

	printf("> ");

	while(fgets(buffer, sizeof(buffer), stdin))
	{
		struct command_struct *command = get_command_struct(buffer);
		if(!command) continue;


		if(strcmp(command->args_malloc_ptr[0], "bye") == 0)
		{
			bye_command(snapshot_head, entry_head);
			free_command(command);
			printf("bye");
			return 0;
		}
		else if(strcmp(command->args_malloc_ptr[0], "help") == 0)
		{
			print_help_string();
		}
		else if(strcmp(command->args_malloc_ptr[0], "list") == 0)
		{
			list_command(command, entry_head, snapshot_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "get") == 0)
		{
			get_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "del") == 0)
		{
			del_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "purge") == 0)
		{
			purge_command(command, entry_head, snapshot_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "set") == 0)
		{
			set_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "push") == 0)
		{
			push_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "append") == 0)
		{
			append_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "pick") == 0)
		{
			pick_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "pluck") == 0)
		{
			pluck_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "pop") == 0)
		{
			pop_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "drop") == 0)
		{
			drop_command(command, snapshot_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "rollback") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "checkout") == 0)
		{
			checkout_command(command, snapshot_head, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "snapshot") == 0)
		{
			snapshot_command(snapshot_head, entry_head, &latest_snapshotID);
		}
		else if(strcmp(command->args_malloc_ptr[0], "min") == 0)
		{
			min_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "max") == 0)
		{
			max_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "sum") == 0)
		{
			sum_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "len") == 0)
		{
			len_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "rev") == 0)
		{
			rev_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "uniq") == 0)
		{
			uniq_command(command, entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "sort") == 0)
		{
			sort_command(command, entry_head);
		}
		printf("\n> ");
		free_command(command);
	}
		bye_command(snapshot_head, entry_head);
	return 0;
}
