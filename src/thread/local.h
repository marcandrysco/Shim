#ifndef THREAD_LOCAL_H
#define THREAD_LOCAL_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct thread_local_t;

/*
 * thread-local function declarations
 */

struct thread_local_t *thread_local_new(void (*destroy)(void *));
void thread_local_delete(struct thread_local_t *local);

void *thread_local_get(struct thread_local_t *local);
void thread_local_set(struct thread_local_t *local, void *value);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
