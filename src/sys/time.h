#ifndef SYS_TIME_H
#define SYS_TIME_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * time function declarations
 */

int64_t sys_time();
int64_t sys_utime();

void sys_sleep(uint64_t sec);
void sys_usleep(uint64_t usec);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
