#include "../common.h"
#include "proc.h"


/*
 * implementation function declarations
 */

_noreturn void _impl_sys_exit(int code);

void _impl_sys_atexit(void (*func)());


/**
 * Exit the process, returning the given exit status.
 *   @status: The exit status.
 */

_export _noreturn
void sys_exit(int status)
{
	_impl_sys_exit(status);
}


/**
 * Add a handler to execute when terminating the program.
 *   @func: The handler function.
 */

_export
void sys_atexit(void (*func)())
{
	_impl_sys_atexit(func);
}
