#ifndef COMMAND_H
#define COMMAND_H

struct command_struct
{
	char *args_malloc_ptr[3];
};

static char *get_arg_from_pointer_malloc_ptr(char *);
static char *get_long_arg_from_pointer_malloc_ptr(char *);
static char *get_pointer_to_arg_ptr(char *, int);
static char *get_arg_malloc_ptr(char *, int);
static int get_num_args(char *);
struct command_struct *get_command_struct(char *);


#endif
