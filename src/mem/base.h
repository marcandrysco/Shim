#ifndef MEM_BASE_H
#define MEM_BASE_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * basic memory function declarations
 */

void mem_zero(void *dest, size_t nbytes);
void mem_set(void *dest, uint8_t byte, size_t nbytes);
void mem_copy(void *restrict dest, const void *restrict src, size_t nbytes);
void mem_move(void *dest, const void *src, size_t nbytes);

int mem_cmp(const void *p1, const void *p2, size_t nbytes);
bool mem_isequal(const void *p1, const void *p2, size_t nbytes);

void *mem_dup(void *ptr, size_t nbytes);

#define mem_getref(type, val) (&(union { type v; }){ .v = (val) })
#define mem_dupval(type, val) mem_dup(&(union { type v; }){ .v = (val) }, sizeof(type))

#define getcontainer(ptr, type, member) ((void *)(ptr) - offsetof(type, member))

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
