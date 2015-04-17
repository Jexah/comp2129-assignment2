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

void append_int_to_entry(int value, entry *entry_ptr)
{
    value *new_value_ptr = malloc(sizeof(value));
    new_value_ptr->value = value;
    append_value_to_entry(new_value_ptr, entry_ptr);
}

void delete_entry_values(entry *target_entry)
{
	value *cursor = target_entry->values->next;
	while(free(cursor->prev), cursor = cursor->next);
    target_entry->values = null;
}

void delete_entry(entry *target_entry)
{
    delete_entry_values(target_entry);
    target_entry->next->prev = target_entry->prev;
    target_entry->prev->next = target_entry->next;
    free(target_entry);
}

void append_entry_to_entry_head(value *new_entry_ptr, entry *entry_head_ptr)
{
    entry *cursor = entry_head_ptr;
	while(cursor->next)
	{
        cursor = cursor->next;
	}
    cursor->next = new_entry_ptr;
    new_entry_ptr->prev = cursor;
}

#endif
