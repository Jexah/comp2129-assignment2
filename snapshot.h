#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#define MAX_KEY_LENGTH 16
#define MAX_LINE_LENGTH 1024

typedef struct command_struct
{
	char *args_malloc_ptr[3];
} command_struct;

typedef enum STATUS { OK, NO_KEY, NO_KEYS, NO_ENTRIES, NO_SNAPSHOTS, NO_ENTRY, NO_SNAPSHOT } STATUS;

char *get_arg_from_pointer_malloc_ptr(char *);
char *get_long_arg_from_pointer_malloc_ptr(char *);
char *get_pointer_to_arg_ptr(char *, int);
char *get_arg_malloc_ptr(char *, int);
int get_num_args(char *);
command_struct *get_command_struct(char *);

typedef struct value value;
typedef struct entry entry;
typedef struct snapshot snapshot;

struct value {
  value* prev;
  value* next;
  int value;
};

struct entry {
  entry* prev;
  entry* next;
  value* values;
  char key[MAX_KEY_LENGTH];
};

struct snapshot {
  snapshot* prev;
  snapshot* next;
  entry* entries;
  int id;
};

void append_value_to_entry(value *, entry *);
void append_int_to_entry(int , entry *);
void delete_entry_values(entry *);
void delete_entry(entry *);
void append_entry_to_entry_head(entry *, entry *);
entry *find_entry_by_key(char *, entry *);
void set_entry_values_by_key(char *, char *, entry *);
void append_entry_values_by_key(char *, char *, entry *);
void delete_entry_values_by_key(char *, entry *);
void delete_entry_by_key(char *, entry *);
void create_entry(char *, entry *);



STATUS append_value_to_entry(value *new_value_ptr, entry *entry_ptr)
{
    value *cursor = entry_ptr->values;
	while(cursor->next)
	{
        cursor = cursor->next;
	}
    cursor->next = new_value_ptr;
    new_value_ptr->prev = cursor;
	return OK;
}

STATUS append_int_to_entry(int number, entry *entry_ptr)
{
    value *new_value_ptr = calloc(sizeof(value), 1);
    new_value_ptr->value = number;
    return append_value_to_entry(new_value_ptr, entry_ptr);
}

STATUS delete_entry_values(entry *target_entry)
{
	if(!target_entry->values)
	{
		return;
	}
	value *cursor = target_entry->values->next;
	while(free(cursor->prev), (cursor = cursor->next));
	return OK;
}

STATUS delete_entry(entry *target_entry)
{
    STATUS delete_success = delete_entry_values(target_entry);
	if(delete_success != OK)
	{
		return delete_success;
	}
    if(target_entry->next) target_entry->next->prev = target_entry->prev;
	if(target_entry->prev) target_entry->prev->next = target_entry->next;
    free(target_entry);
	return OK;
}

STATUS free_entries_from_head(entry *entry_head)
{
	entry *cursor = entry_head;
	while(cursor)
	{
		entry_head = cursor;
		STATUS delete_status = delete_entry(entry_head);
		if(delete_status != OK)
		{
			return delete_status;
		}
		cursor = cursor->next;
	}
	return OK;
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
	if(method_status != OK)
	{
		return method_status;
	}
	method_status = create_entry(key, entry_head);
	if(method_status != OK)
	{
		return method_status;
	}
	method_status = append_entry_values_by_key(key, values, entry_head);
	if(method_status != OK)
	{
		return method_status;
	}
}

void print_values_in_entry(entry *entry_head)
{
	if(!entry_head)
	{
		printf("no such entry\n");
	}
	value *value_cursor = entry_head->values->next;
	printf("[");
	while(value_cursor)
	{
		printf("%d", value_cursor->value);
		value_cursor = value_cursor->next;
		if(value_cursor)
		{
			printf(" ");
		}
	}
	printf("]");
}

STATUS append_entry_values_by_key(char *key, char *values, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		return NO_KEY;
	}
	while(values)
	{
		STATUS append_int_status = append_int_to_entry(atoi(values), entry_ptr);
		if(append_int_status != OK)
		{
			return append_int_status;
		}
		char *next_space = strchr(values+1, ' ');
		values = (next_space ? next_space : strchr(values+1, '\n'));
	};
	return OK;
}

STATUS create_entry_if_not_exist(char *key, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		entry_ptr = calloc(sizeof(entry), 1);
		value *entry_ptr_values_head = calloc(sizeof(value), 1);
		entry_ptr->values = calloc(sizeof(&entry_ptr_values_head), 1);
		entry_ptr->values = entry_ptr_values_head;
		strncpy(entry_ptr->key, key, MAX_KEY_LENGTH);
		STATUS append_entry_status = append_entry_to_entry_head(entry_ptr, entry_head);
		if(append_entry_status != OK)
		{
			return append_entry_status;
		}
	}
}

STATUS delete_entry_values_by_key(char *key, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		return NO_KEY;
	}
	STATUS delete_entry_values_status = delete_entry_values(entry_ptr);
	if(delete_entry_values_status != OK)
	{
		return delete_entry_values_status;
	}
	return OK;
}

STATUS delete_entry_by_key(char *key, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		return NO_KEY;
	}
	STATUS delete_entry_status = delete_entry(entry_ptr);
	if(delete_entry_status != OK)
	{
		return delete_entry_status;
	}
}

STATUS purge_entry(char *key, entry *entry_head, snapshot *snapshot_head)
{
	STATUS del_command_status = del_command(command, entry_head);
	if(del_command_status != OK && del_command_status != NO_KEY)
	{
		return del_command_status;
	}
	snapshot *snapshot_cursor = snapshot_head->next;
	while(snapshot_cursor)
	{
		del_command_status = delete_entry_by_key(key, snapshot_cursor->entries);
		if(del_command_status != OK && del_command_status != NO_KEY)
		{
			return del_command_status;
		}
		snapshot_cursor = snapshot_cursor->next;
	}
	return OK;
}

#endif
