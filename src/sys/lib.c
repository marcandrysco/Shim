#include "../common.h"
#include "lib.h"


/*
 * implementation function declarations
 */

struct sys_dynlib_t *_impl_sys_dynlib_open(const char *path);
void _impl_sys_dynlib_close(struct sys_dynlib_t *dynlib);
void *_impl_sys_dynlib_symbol(struct sys_dynlib_t *dynlib, const char *restrict name);


/**
 * Open a dynamic library.
 *   @path: The path.
 *   &returns: The dynamic library.
 */

struct sys_dynlib_t *sys_dynlib_open(const char *path)
{
	return _impl_sys_dynlib_open(path);
}

/**
 * Close a dynamic library.
 *   @dynlib: The dynamic library.
 */

void sys_dynlib_close(struct sys_dynlib_t *dynlib)
{
	_impl_sys_dynlib_close(dynlib);
}

/**
 * Lookup a symbol in a dynamic library.
 *   @dynlib: The dynamic library.
 *   @name: The symbol name.
 *   &returns: The symbol or null.
 */

void *sys_dynlib_symbol(struct sys_dynlib_t *dynlib, const char *restrict name)
{
	return _impl_sys_dynlib_symbol(dynlib, name);
}
