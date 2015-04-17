#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snapshot.h"

#include "command.c"
#include "list.c"



// most recently added
entry* entry_head = NULL;
snapshot* snapshot_head = NULL;



int main(void) {

	char buffer[MAX_LINE_LENGTH];

	while(fgets(buffer, sizeof(buffer), stdin))
	{
		struct command_struct *command = get_command_struct(buffer);
		if(!command) continue;
		if(command->args_malloc_ptr[0]) printf("Arg1: %s\n", command->args_malloc_ptr[0]);
		if(command->args_malloc_ptr[1]) printf("Arg2: %s\n", command->args_malloc_ptr[1]);
		if(command->args_malloc_ptr[2]) printf("Arg3: %s\n", command->args_malloc_ptr[2]);
	}

	return 0;
}
