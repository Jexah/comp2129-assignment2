#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#define MAX_KEY_LENGTH 16
#define MAX_LINE_LENGTH 1024

typedef struct command_struct
{
	char *args_malloc_ptr[3];
} command_struct;

typedef enum STATUS { OK, NO_KEY, NO_KEYS, NO_ENTRIES, NO_SNAPSHOTS, NO_ENTRY, NO_SNAPSHOT, MALLOC_FAILED } STATUS;

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

// Command interpretation
char *get_arg_from_pointer_malloc_ptr(char *);
char *get_long_arg_from_pointer_malloc_ptr(char *);
char *get_pointer_to_arg_ptr(char *, int);
char *get_arg_malloc_ptr(char *, int);
int get_num_args(char *);
command_struct *get_command_struct(char *);

// Database operations
STATUS get_new_value(int, value **);
STATUS append_value_to_entry(value *, entry *);
STATUS append_int_to_entry(int, entry *);
STATUS delete_entry_values(entry *);
STATUS delete_entry(entry *);
STATUS free_entries_from_head(entry *);
STATUS append_entry_to_entry_head(entry *, entry *);
entry *find_entry_by_key(char *, entry *);
STATUS set_entry_values_by_key(char *, char *, entry *);
void print_values_in_entry(entry *);
STATUS append_entry_values_by_key(char *, char *, entry *);
STATUS create_entry_if_not_exist(char *, entry *entry_head);
STATUS delete_entry_values_by_key(char *, entry *);
STATUS delete_entry_by_key(char *, entry *);
STATUS purge_entry(char *, entry *, snapshot *);
STATUS push_int_on_entry(int, entry *);
STATUS push_value_on_entry(value *, entry *);

// Options
void get_command(command_struct *, entry *);
void del_command(command_struct *, entry *);
void purge_command(command_struct *, entry *, snapshot *);
STATUS list_command_keys(entry *);
STATUS list_command_entries(entry *);
STATUS list_command_snapshots(snapshot *);
void list_command(command_struct *, entry *, snapshot *);
void set_command(command_struct *, entry *);

#endif
