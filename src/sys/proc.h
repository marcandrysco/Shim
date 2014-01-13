#ifndef SYS_PROC_H
#define SYS_PROC_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * process function declarations
 */

_noreturn void sys_exit(int status);

void sys_atexit(void (*func)());

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
