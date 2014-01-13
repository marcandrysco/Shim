#include "../common.h"
#include "lock.h"


/*
 * implementation function declarations
 */

struct thread_mutex_t *_impl_thread_mutex_new(struct thread_mutex_attr_t *attr);
void _impl_thread_mutex_delete(struct thread_mutex_t *mutex);

void _impl_thread_mutex_lock(struct thread_mutex_t *mutex);
bool _impl_thread_mutex_trylock(struct thread_mutex_t *mutex);
void _impl_thread_mutex_unlock(struct thread_mutex_t *mutex);

struct thread_rwlock_t *_impl_thread_rwlock_new(struct thread_rwlock_attr_t *attr);
void _impl_thread_rwlock_delete(struct thread_rwlock_t *lock);

void _impl_thread_rwlock_rdlock(struct thread_rwlock_t *lock);
bool _impl_thread_rwlock_tryrdlock(struct thread_rwlock_t *lock);
void _impl_thread_rwlock_rdunlock(struct thread_rwlock_t *lock);

void _impl_thread_rwlock_wrlock(struct thread_rwlock_t *lock);
bool _impl_thread_rwlock_trywrlock(struct thread_rwlock_t *lock);
void _impl_thread_rwlock_wrunlock(struct thread_rwlock_t *lock);


/**
 * Create a new mutex.
 *   @attr: The mutex attributes.
 *   &returns: The mutex.
 */

struct thread_mutex_t *thread_mutex_new(struct thread_mutex_attr_t *attr)
{
	return _impl_thread_mutex_new(attr);
}

/**
 * Delete a mutex.
 *   @mutex: The mutex.
 */

void thread_mutex_delete(struct thread_mutex_t *mutex)
{
	_impl_thread_mutex_delete(mutex);
}


/**
 * Lock the mutex.
 *   @mutex: The mutex.a
 */

void thread_mutex_lock(struct thread_mutex_t *mutex)
{
	_impl_thread_mutex_lock(mutex);
}

/**
 * Try to lock the mutex.
 *   @mutex: The mutex.
 *   &returns: True if locked, false if lock failed.
 */

bool thread_mutex_trylock(struct thread_mutex_t *mutex)
{
	return _impl_thread_mutex_trylock(mutex);
}

/**
 * Unlock the mutex.
 *   @mutex: The mutex.a
 */

void thread_mutex_unlock(struct thread_mutex_t *mutex)
{
	_impl_thread_mutex_unlock(mutex);
}
