#ifndef MEM_MANAGE_H
#define MEM_MANAGE_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * memory function declarations
 */

void *_mem_alloc(size_t nbytes);
void *_mem_realloc(void *ptr, size_t nbytes);
void _mem_free(void *ptr);
void _mem_delete(void *ptr);

void *_mem_alloc_dbg(size_t nbytes, const char *file, unsigned int line);
void *_mem_realloc_dbg(void *ptr, size_t nbytes, const char *file, unsigned int line);
void _mem_free_dbg(void *ptr);
void _mem_delete_dbg(void *ptr);

#ifdef _debug
#	define mem_alloc(nbytes) _mem_alloc_dbg(nbytes, __FILE__, __LINE__)
#	define mem_realloc(ptr, nbytes) _mem_realloc_dbg(ptr, nbytes, __FILE__, __LINE__)
#	define mem_free _mem_free_dbg
#	define mem_delete _mem_delete_dbg
#else
#	define mem_alloc(nbytes) _mem_alloc(nbytes)
#	define mem_realloc(ptr, nbytes) _mem_realloc(ptr, nbytes)
#	define mem_free _mem_free
#	define mem_delete _mem_delete
#endif

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
