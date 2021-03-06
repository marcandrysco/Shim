#include "../common.h"
#include "lock.h"


/*
 * implementation function declarations
 */

struct thread_mutex_t _impl_thread_mutex_new(struct thread_mutex_attr_t *attr);
void _impl_thread_mutex_delete(struct thread_mutex_t *mutex);

void _impl_thread_mutex_lock(struct thread_mutex_t *mutex);
bool _impl_thread_mutex_trylock(struct thread_mutex_t *mutex);
void _impl_thread_mutex_unlock(struct thread_mutex_t *mutex);

struct thread_rwlock_t _impl_thread_rwlock_new(struct thread_rwlock_attr_t *attr);
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

_export
struct thread_mutex_t thread_mutex_new(struct thread_mutex_attr_t *attr)
{
	return _impl_thread_mutex_new(attr);
}

/**
 * Delete a mutex.
 *   @mutex: The mutex.
 */

_export
void thread_mutex_delete(struct thread_mutex_t *mutex)
{
	_impl_thread_mutex_delete(mutex);
}


/**
 * Lock the mutex.
 *   @mutex: The mutex.a
 */

_export
void thread_mutex_lock(struct thread_mutex_t *mutex)
{
	_impl_thread_mutex_lock(mutex);
}

/**
 * Try to lock the mutex.
 *   @mutex: The mutex.
 *   &returns: True if locked, false if lock failed.
 */

_export
bool thread_mutex_trylock(struct thread_mutex_t *mutex)
{
	return _impl_thread_mutex_trylock(mutex);
}

/**
 * Unlock the mutex.
 *   @mutex: The mutex.a
 */

_export
void thread_mutex_unlock(struct thread_mutex_t *mutex)
{
	_impl_thread_mutex_unlock(mutex);
}


/**
 * Create a new read-write lock.
 *   @attr: The read-write lock attributes.
 *   &returns: The read-write lock.
 */

_export
struct thread_rwlock_t thread_rwlock_new(struct thread_rwlock_attr_t *attr)
{
	return _impl_thread_rwlock_new(attr);
}

/**
 * Delete a read-write lock.
 *   @lock: The read-write lock.
 */

_export
void thread_rwlock_delete(struct thread_rwlock_t *lock)
{
	_impl_thread_rwlock_delete(lock);
}


/**
 * Lock the read-write lock for reading.
 *   @lock: The read-write lock.
 */

_export
void thread_rwlock_rdlock(struct thread_rwlock_t *lock)
{
	_impl_thread_rwlock_rdlock(lock);
}

/**
 * Try to lock the read-write lock for reading.
 *   @lock: The read-write lock.
 *   &returns: True if locked, false if lock failed.
 */

_export
bool thread_rwlock_tryrdlock(struct thread_rwlock_t *lock)
{
	return _impl_thread_rwlock_tryrdlock(lock);
}

/**
 * Unlock the read-write lock for reading.
 *   @lock: The read-write lock.
 */

_export
void thread_rwlock_rdunlock(struct thread_rwlock_t *lock)
{
	_impl_thread_rwlock_rdunlock(lock);
}


/**
 * Lock the read-write lock for writing.
 *   @lock: The read-write lock.
 */

_export
void thread_rwlock_wrlock(struct thread_rwlock_t *lock)
{
	_impl_thread_rwlock_wrlock(lock);
}

/**
 * Try to lock the read-write lock for writing.
 *   @lock: The read-write lock.
 *   &returns: True if locked, false if lock failed.
 */

_export
bool thread_rwlock_trywrlock(struct thread_rwlock_t *lock)
{
	return _impl_thread_rwlock_trywrlock(lock);
}

/**
 * Unlock the read-write lock for writing.
 *   @lock: The read-write lock.
 */

_export
void thread_rwlock_wrunlock(struct thread_rwlock_t *lock)
{
	_impl_thread_rwlock_wrunlock(lock);
}
