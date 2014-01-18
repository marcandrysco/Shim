#ifndef MEM_BASE_H
#define MEM_BASE_H
/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * basic memory function declarations
 */

void mem_copy(void *restrict dest, const void *restrict src, size_t nbytes);

bool mem_isequal(const void *restrict p1, const void *restrict p2, size_t nbytes);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
