#ifndef THREAD_POSIX_LOCK_H
#define THREAD_POSIX_LOCK_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Mutex structure.
 *   @lock: The pthread lock.
 */

struct thread_mutex_t {
	pthread_mutex_t lock;
};

/**
 * Read-write lock structure.
 *   @rw: The pthread rwlock.
 */

struct thread_rwlock_t {
	pthread_rwlock_t rwlock;
};

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
