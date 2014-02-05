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

struct thread_mutex_t thread_mutex_new(struct thread_mutex_attr_t *attr);
void thread_mutex_delete(struct thread_mutex_t *mutex);

void thread_mutex_lock(struct thread_mutex_t *mutex);
bool thread_mutex_trylock(struct thread_mutex_t *mutex);
void thread_mutex_unlock(struct thread_mutex_t *mutex);

struct thread_rwlock_t thread_rwlock_new(struct thread_rwlock_attr_t *attr);
void thread_rwlock_delete(struct thread_rwlock_t *lock);

void thread_rwlock_rdlock(struct thread_rwlock_t *lock);
bool thread_rwlock_tryrdlock(struct thread_rwlock_t *lock);
void thread_rwlock_rdunlock(struct thread_rwlock_t *lock);

void thread_rwlock_wrlock(struct thread_rwlock_t *lock);
bool thread_rwlock_trywrlock(struct thread_rwlock_t *lock);
void thread_rwlock_wrunlock(struct thread_rwlock_t *lock);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
