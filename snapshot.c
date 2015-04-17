#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snapshot.h"

#include "command.c"
#include "list.c"

void print_help_string()
{
	printf(\
		"\
\	\	\	BYE clear database and exit
\	\	\	HELP display this help message

\	\	\	LIST KEYS displays all keys in current state
\	\	\	LIST ENTRIES displays all entries in current state
\	\	\	LIST SNAPSHOTS displays all snapshot ids

\	\	\	GET <key> displays entry values
\	\	\	DEL <key> deletes entry from current state
\	\	\	PURGE <key> deletes entry from current state and snapshots

\	\	\	SET <key> <value ...> sets entry values
\	\	\	PUSH <key> <value ...> pushes each value to the front one at a time
\	\	\	APPEND <key> <value ...> append each value to the back one at a time

\	\	\	PICK <key> <index> displays entry value at index
\	\	\	PLUCK <key> <index> displays and removes entry value at index
\	\	\	POP <key> displays and removes the front entry value

\	\	\	DROP <id> deletes snapshot
\	\	\	ROLLBACK <id> restores to snapshot and deletes newer snapshots
\	\	\	CHECKOUT <id> replaces current state with a copy of snapshot
\	\	\	SNAPSHOT saves the current state as a snapshot

\	\	\	MIN <key> displays minimum entry value
\	\	\	MAX <key> displays maximum entry value
\	\	\	SUM <key> displays sum of entry values
\	\	\	LEN <key> displays number of entry values

\	\	\	REV <key> reverses order of entry values
\	\	\	UNIQ <key> removes repeated adjacent entry values
\	\	\	SORT <key> sorts entry values in ascending order
\	\	"
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
		char command_string[strlen(command->args_malloc_ptr[0])];
		if(command_string) printf("Arg1: %s\n", command_string);
		if(command->args_malloc_ptr[1]) printf("Arg2: %s\n", command->args_malloc_ptr[1]);
		if(command->args_malloc_ptr[2]) printf("Arg3: %s\n", command->args_malloc_ptr[2]);

		strcpy(command_string, command->args_malloc_ptr[0]);

		if(strcmp(command_string, "help"))
		{
			print_help_string();
		}
		else if(strcmp(command_string, "bye"))
		{

		}
		else if(strcmp(command_string, "list"))
		{

		}
		else if(strcmp(command_string, "get"))
		{

		}
		else if(strcmp(command_string, "append"))
		{

		}
		else if(strcmp(command_string, "pick"))
		{

		}
		else if(strcmp(command_string, "pluck"))
		{

		}
		else if(strcmp(command_string, "pop"))
		{

		}
		else if(strcmp(command_string, "drop"))
		{

		}
		else if(strcmp(command_string, "checkout"))
		{

		}
		else if(strcmp(command_string, "snapshot"))
		{

		}
		else if(strcmp(command_string, "min"))
		{

		}
		else if(strcmp(command_string, "max"))
		{

		}
		else if(strcmp(command_string, "sum"))
		{

		}
		else if(strcmp(command_string, "len"))
		{

		}
		else if(strcmp(command_string, "rev"))
		{

		}
		else if(strcmp(command_string, "uniq"))
		{

		}
		else if(strcmp(command_string, "sort"))
		{

		}
	}

	return 0;
}
