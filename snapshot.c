#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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



int main(void) {

	char buffer[MAX_LINE_LENGTH];

	printf("> ");

	while(fgets(buffer, sizeof(buffer), stdin))
	{
		struct command_struct *command = get_command_struct(buffer);
		if(!command) continue;

		if(command->args_malloc_ptr[0]) printf("Arg1: %s\n", command->args_malloc_ptr[0]);
		if(command->args_malloc_ptr[1]) printf("Arg2: %s\n", command->args_malloc_ptr[1]);
		if(command->args_malloc_ptr[2]) printf("Arg3: %s\n", command->args_malloc_ptr[2]);


		if(strcmp(command->args_malloc_ptr[0], "help"))
		{
			print_help_string();
		}
		else if(strcmp(command->args_malloc_ptr[0], "bye"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "list"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "get"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "append"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "pick"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "pluck"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "pop"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "drop"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "checkout"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "snapshot"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "min"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "max"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "sum"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "len"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "rev"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "uniq"))
		{

		}
		else if(strcmp(command->args_malloc_ptr[0], "sort"))
		{

		}
	}

	return 0;
}
