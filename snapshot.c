#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void str_tolower(char *str)
{
	for(int i = 0; str[i]; ++i){
		str[i] = tolower(str[i]);
	}
}


#include "snapshot.h"

#include "command.c"
#include "list.c"

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

// most recently added
entry* entry_head = NULL;
snapshot* snapshot_head = NULL;

entry *find_entry_by_key(char *key_ptr)
{
	entry *cursor_ptr = entry_head
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

void set_entry_values_by_key(char *key, char *values)
{
	delete_entry_values_by_key(key);
	append_entry_values_by_key(key, values);
}

void append_entry_values_by_key(char *key, char *values)
{
	entry *entry_ptr = find_entry_by_key(key);
	if(!entry_ptr)
	{
		entry_ptr = malloc(sizeof(entry));
		append_entry_to_entries(entry_ptr, entry_head);
	}
	while(append_int_to_entry(atoi(values), entry_ptr), (values = strchr(values, ' ')) || (values = strchr(values, '\n')));
	printf("success append values");
}

void delete_entry_values_by_key(char *key)
{
	entry *entry_ptr = find_entry_by_key(key);
	if(!entry_ptr)
	{
		printf("key does not exist");
		return;
	}
	delete_entry_values(entry_ptr);
	printf("success delete entry values")
}

void delete_entry_by_key(char *key)
{
	entry *entry_ptr = find_entry_by_key(key);
	if(!entry_ptr)
	{
		printf("key does not exist");
		return;
	}
	delete_entry(entry_ptr);
	printf("success delete entry")
}

int main(void) {

	char buffer[MAX_LINE_LENGTH];

	printf("> ");

	while(fgets(buffer, sizeof(buffer), stdin))
	{
		struct command_struct *command = get_command_struct(buffer);
		if(!command) continue;

		if(command->args_malloc_ptr[0]) printf("Arg1: '%s'\n", command->args_malloc_ptr[0]);
		if(command->args_malloc_ptr[1]) printf("Arg2: %s\n", command->args_malloc_ptr[1]);
		if(command->args_malloc_ptr[2]) printf("Arg3: %s\n", command->args_malloc_ptr[2]);


		if(strcmp(command->args_malloc_ptr[0], "bye") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "help") == 0)
		{
			print_help_string();
		}
		else if(strcmp(command->args_malloc_ptr[0], "list") == 0)
		{
			str_tolower(command->args_malloc_ptr[1]);
			if(strcmp(command->args_malloc_ptr[1], "keys") == 0)
			{
				if(entry_head && entry_head->values)
				{

				}
				else
				{
					printf("no keys");
				}
			}
			else if(strcmp(command->args_malloc_ptr[1], "entries") == 0)
			{
				if(entry_head)
				{

				}
				else
				{
					printf("no entries");
				}
			}
			else if(strcmp(command->args_malloc_ptr[1], "snapshots") == 0)
			{
				if(snapshot_head)
				{

				}
				else
				{
					printf("no snapshots");
				}
			}
		}
		else if(strcmp(command->args_malloc_ptr[0], "get") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "del") == 0)
		{
			delete_entry_values_by_key(command->args_malloc_ptr[1]);
		}
		else if(strcmp(command->args_malloc_ptr[0], "purge") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "set") == 0)
		{
			set_entry_values_by_key(command->args_malloc_ptr[1], command->args_malloc_ptr[2]);
		}
		else if(strcmp(command->args_malloc_ptr[0], "push") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "append") == 0)
		{
			append_entry_values_by_key(command->args_malloc_ptr[1], command->args_malloc_ptr[2]);
		}
		else if(strcmp(command->args_malloc_ptr[0], "pick") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "pluck") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "pop") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "drop") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "rollback") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "checkout") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "snapshot") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "min") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "max") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "sum") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "len") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "rev") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "uniq") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "sort") == 0)
		{

		}
		for(int i = 0; i < 3; ++i)
		{
			free(command->args_malloc_ptr[i]);
		}
		free(command);
	}

	return 0;
}
