#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#define MAX_KEY_LENGTH 16
#define MAX_LINE_LENGTH 1024

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



void append_value_to_entry(value *new_value_ptr, entry *entry_ptr)
{
    value *cursor = entry_ptr->values;
	while(cursor->next)
	{
        cursor = cursor->next;
	}
    cursor->next = new_value_ptr;
    new_value_ptr->prev = cursor;
}

void append_int_to_entry(int number, entry *entry_ptr)
{
    value *new_value_ptr = malloc(sizeof(value));
    new_value_ptr->value = number;
    append_value_to_entry(new_value_ptr, entry_ptr);
}

void delete_entry_values(entry *target_entry)
{
	value *cursor = target_entry->values->next;
	while(free(cursor->prev), (cursor = cursor->next));
    target_entry->values = 0;
}

void delete_entry(entry *target_entry)
{
    delete_entry_values(target_entry);
    target_entry->next->prev = target_entry->prev;
    target_entry->prev->next = target_entry->next;
    free(target_entry);
}

void append_entry_to_entry_head(entry *new_entry_ptr, entry *entry_head_ptr)
{
    entry *cursor = entry_head_ptr;
	while(cursor->next)
	{
        cursor = cursor->next;
	}
    cursor->next = new_entry_ptr;
    new_entry_ptr->prev = cursor;
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

void set_entry_values_by_key(char *key, char *values, entry *entry_head)
{
	delete_entry_values_by_key(key, entry_head);
	append_entry_values_by_key(key, values, entry_head);
}

void append_entry_values_by_key(char *key, char *values, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		entry_ptr = malloc(sizeof(entry));
		append_entry_to_entries(entry_ptr, entry_head);
	}
	while(append_int_to_entry(atoi(values), entry_ptr), (values = strchr(values, ' ')) || (values = strchr(values, '\n')));
	printf("success append values");
}

void delete_entry_values_by_key(char *key, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		printf("key does not exist");
		return;
	}
	delete_entry_values(entry_ptr);
	printf("success delete entry values");
}

void delete_entry_by_key(char *key, entry *entry_head)
{
	entry *entry_ptr = find_entry_by_key(key, entry_head);
	if(!entry_ptr)
	{
		printf("key does not exist");
		return;
	}
	delete_entry(entry_ptr);
	printf("success delete entry");
}

#endif
