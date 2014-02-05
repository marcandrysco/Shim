#ifndef THREAD_POSIX_DEFS_H
#define THREAD_POSIX_DEFS_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * common headers
 */

#include <pthread.h>


/**
 * Mutex structure.
 *   @lock: The pthread lock.
 */

struct thread_mutex_t {
	pthread_mutex_t lock;
};

/**
 * Read-write lock structure.
 *   @rwlock: The pthread rwlock.
 */

struct thread_rwlock_t {
	pthread_rwlock_t rwlock;
};

/**
 * Condition variable structure.
 *   @var: The pthread condition variable.
 */

struct thread_cond_t {
	pthread_cond_t var;
};

/**
 * Single run structure.
 *   @once: The once.
 */

struct thread_once_t {
	pthread_once_t once;
};


/*
 * static initailizer definitions
 */

#define THREAD_MUTEX_INIT	{ PTHREAD_MUTEX_INITIALIZER }
#define THREAD_RWLOCK_INIT	{ PTHREAD_RWLOCK_INITIALIZER }
#define THREAD_ONCE_INIT	{ PTHREAD_ONCE_INIT }

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
