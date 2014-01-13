#ifndef THREAD_LOCK_H
#define THREAD_LOCK_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct thread_mutex_t;
struct thread_mutex_attr_t;
struct thread_rwlock_t;
struct thread_rwlock_attr_t;

/*
 * mutex function declarations
 */

struct thread_mutex_t *_impl_thread_mutex_new(struct thread_mutex_attr_t *attr);
void thread_mutex_delete(struct thread_mutex_t *mutex);

void thread_mutex_lock(struct thread_mutex_t *mutex);
bool thread_mutex_trylock(struct thread_mutex_t *mutex);
void thread_mutex_unlock(struct thread_mutex_t *mutex);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
