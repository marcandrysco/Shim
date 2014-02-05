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

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
