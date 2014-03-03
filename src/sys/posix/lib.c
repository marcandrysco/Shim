#include "../../common.h"
#include <dlfcn.h>
#include "../../debug/exception.h"
#include "../../mem/manage.h"


/**
 * Dynamic library structure.
 *   @ref: the reference.
 */

struct sys_dynlib_t {
	void *ref;
};

/*
 * global variables
 */

_export const char sys_dynlib_ext[] = ".so";


/**
 * Open a dynamic library.
 *   @path: The path.
 *   &returns: The dynamic library.
 */

struct sys_dynlib_t *_impl_sys_dynlib_open(const char *path)
{
	void *ref;
	struct sys_dynlib_t *dynlib;

	ref = dlopen(path, RTLD_NOW | RTLD_LOCAL);
	if(ref == NULL)
		throw("Failed to load '%s'. %s.", path, dlerror());

	dynlib = mem_alloc(sizeof(struct sys_dynlib_t));
	dynlib->ref = ref;

	return dynlib;
}

/**
 * Close a dynamic library.
 *   @dynlib: The dynamic library.
 */

void _impl_sys_dynlib_close(struct sys_dynlib_t *dynlib)
{
	dlclose(dynlib->ref);
	mem_free(dynlib);
}

/**
 * Lookup a symbol in a dynamic library.
 *   @dynlib: The dynamic library.
 *   @name: The symbol name.
 *   &returns: The symbol or null.
 */

void *_impl_sys_dynlib_symbol(struct sys_dynlib_t *dynlib, const char *restrict name)
{
	return dlsym(dynlib->ref, name);
}
