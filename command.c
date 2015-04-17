#include <stdlib.h>
#include <string.h>

#include "command.h"

static char *get_arg_from_pointer_malloc_ptr(char *start_ptr)
{
	char *return_ptr = calloc((strchr(start_ptr, ' ') - start_ptr) + 1, 1);
	char *rolling_ptr = return_ptr;
	while(*start_ptr != ' ' && (*rolling_ptr++ = *start_ptr++));
	return return_ptr;
}

static char *get_long_arg_from_pointer_malloc_ptr(char *start_ptr)
{
	char *return_malloc_ptr = calloc((strchr(start_ptr, ' ') - start_ptr) + 1, 1);
	char *rolling_ptr = return_malloc_ptr;
	while((*rolling_ptr++ = *start_ptr++));
	return return_malloc_ptr;
}

static char *get_pointer_to_arg_ptr(char *start_ptr, int arg)
{
	for(int i = 0; i < arg; ++i)
	{
		if(i != arg)
		{
			start_ptr = strchr(start_ptr, ' ') + 1;
		}
	}
	return start_ptr;
}

static char *get_arg_malloc_ptr(char *input_ptr, int arg)
{
	return get_arg_from_pointer_malloc_ptr(get_pointer_to_arg_ptr(input_ptr, arg));
}

static int get_num_args(char *start_ptr)
{
	int num_args = (!!*start_ptr);
	while(*start_ptr++)
	{
		num_args += *start_ptr == ' ';
	}
	return num_args;
}

struct command_struct *get_command_struct(char *input_ptr)
{
	if(!*input_ptr) return 0;
	struct command_struct *malloc_ptr = calloc(sizeof(struct command_struct), 1);
	int num_args = get_num_args(input_ptr);
	for(int i = 0; i < (num_args < 3 ? num_args : 3); ++i)
	{
		malloc_ptr->args_malloc_ptr[i] = get_arg_malloc_ptr(input_ptr, i);
	}
	for(int i = 0; i < 3 - num_args; ++i)
	{
		malloc_ptr->args_malloc_ptr[i] = 0;
	}
	return malloc_ptr;
}
