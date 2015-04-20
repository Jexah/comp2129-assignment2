#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "snapshot.h"



// **************************************************************
// *********************   Helper module    *********************
// **************************************************************
// Notes:
//			None

// Helper function to convert a string to lowercase.
void str_tolower(char *str)
{
	while(*str++)
	{
		*str = tolower(*str);
	}
}

// //////////////////////////////////////////////////////////////
// /////////////////////   Helper module    /////////////////////
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

// //////////////////////////////////////////////////////////////
// /////////////////////   Options module   /////////////////////
// //////////////////////////////////////////////////////////////

// most recently added
entry* entry_head = NULL;
snapshot* snapshot_head = NULL;



int main(void) {

	entry_head = malloc(sizeof(entry));
	snapshot_head = malloc(sizeof(snapshot));

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
				if(entry_head->next)
				{
					entry *cursor = entry_head->next;
					printf("[");
					while(cursor)
					{
						printf("'%s'", cursor->key);
						if(cursor->next)
						{
							printf(" ");
						}
						cursor = cursor->next;
					}
					printf("]");
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
			delete_entry_values_by_key(command->args_malloc_ptr[1], entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "purge") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "set") == 0)
		{
			set_entry_values_by_key(command->args_malloc_ptr[1], command->args_malloc_ptr[2], entry_head);
		}
		else if(strcmp(command->args_malloc_ptr[0], "push") == 0)
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "append") == 0)
		{
			append_entry_values_by_key(command->args_malloc_ptr[1], command->args_malloc_ptr[2], entry_head);
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
		free_command(command);
	}

	return 0;
}
