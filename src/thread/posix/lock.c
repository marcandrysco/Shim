#include "../../common.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "../../debug/exception.h"
#include "../../mem/manage.h"
#include "../lock.h"
#include "defs.h"


/**
 * Create a new mutex.
 *   @attr: The mutex attributes.
 *   &returns: The mutex.
 */

struct thread_mutex_t *_impl_thread_mutex_new(struct thread_mutex_attr_t *attr)
{
	int err;
	struct thread_mutex_t *mutex;

	mutex = mem_alloc(sizeof(struct thread_mutex_t));
	err = pthread_mutex_init(&mutex->lock, NULL);
	if(err != 0)
		throw("Failed to create mutex. %s.", strerror(err));

	return mutex;
}

/**
 * Delete a mutex.
 *   @mutex: The mutex.
 */

void _impl_thread_mutex_delete(struct thread_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_destroy(&mutex->lock);
	if(err != 0)
		throw("Failed to delete mutex. %s.", strerror(err));
}


/**
 * Lock the mutex.
 *   @mutex: The mutex.a
 */

void _impl_thread_mutex_lock(struct thread_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_lock(&mutex->lock);
	if(err != 0)
		throw("Failed to lock mutex. %s.", strerror(err));
}

/**
 * Try to lock the mutex.
 *   @mutex: The mutex.
 *   &returns: True if locked, false if lock failed.
 */

bool _impl_thread_mutex_trylock(struct thread_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_trylock(&mutex->lock);
	if(err == EBUSY)
		return false;
	else if(err != 0)
		throw("Failed to lock mutex. %s.", strerror(err));

	return true;
}

/**
 * Unlock the mutex.
 *   @mutex: The mutex.a
 */

void _impl_thread_mutex_unlock(struct thread_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_unlock(&mutex->lock);
	if(err != 0)
		throw("Failed to unlock mutex. %s.", strerror(err));
}


/**
 * Create a new read-write lock.
 *   @attr: The read-write lock attributes.
 *   &returns: The read-write lock.
 */

struct thread_rwlock_t *_impl_thread_rwlock_new(struct thread_rwlock_attr_t *attr)
{
	int err;
	struct thread_rwlock_t *lock;

	lock = mem_alloc(sizeof(struct thread_rwlock_t));
	err = pthread_rwlock_init(&lock->rwlock, NULL);
	if(err != 0)
		throw("Failed to create read-write lock. %s.", strerror(err));

	return lock;
}

/**
 * Delete a read-write lock.
 *   @lock: The read-write lock.
 */

void _impl_thread_rwlock_delete(struct thread_rwlock_t *lock)
{
	int err;

	err = pthread_rwlock_destroy(&lock->rwlock);
	if(err != 0)
		throw("Failed to delete read-write lock. %s.", strerror(err));
}


/**
 * Lock the read-write lock for reading.
 *   @lock: The read-write lock.
 */

void _impl_thread_rwlock_rdlock(struct thread_rwlock_t *lock)
{
	int err;

	err = pthread_rwlock_rdlock(&lock->rwlock);
	if(err != 0)
		throw("Failed to lock the read-write lock for reading. %s.", strerror(err));
}

/**
 * Try to lock the read-write lock for reading.
 *   @lock: The read-write lock.
 *   &returns: True if locked, false if lock failed.
 */

bool _impl_thread_rwlock_tryrdlock(struct thread_rwlock_t *lock)
{
	int err;

	err = pthread_rwlock_tryrdlock(&lock->rwlock);
	if(err == EBUSY)
		return false;
	else if(err != 0)
		throw("Failed to lock the read-write lock for reading. %s.", strerror(err));

	return true;
}

/**
 * Unlock the read-write lock for reading.
 *   @lock: The read-write lock.
 */

void _impl_thread_rwlock_rdunlock(struct thread_rwlock_t *lock)
{
	int err;

	err = pthread_rwlock_unlock(&lock->rwlock);
	if(err != 0)
		throw("Failed to unlock the read-write lock for reading. %s.", strerror(err));
}


/**
 * Lock the read-write lock for writing.
 *   @lock: The read-write lock.
 */

void _impl_thread_rwlock_wrlock(struct thread_rwlock_t *lock)
{
	int err;

	err = pthread_rwlock_wrlock(&lock->rwlock);
	if(err != 0)
		throw("Failed to lock the read-write lock for writing. %s.", strerror(err));
}

/**
 * Try to lock the read-write lock for writing.
 *   @lock: The read-write lock.
 *   &returns: True if locked, false if lock failed.
 */

bool _impl_thread_rwlock_trywrlock(struct thread_rwlock_t *lock)
{
	int err;

	err = pthread_rwlock_trywrlock(&lock->rwlock);
	if(err == EBUSY)
		return false;
	else if(err != 0)
		throw("Failed to lock the read-write lock for writing. %s.", strerror(err));

	return true;
}

/**
 * Unlock the read-write lock for writing.
 *   @lock: The read-write lock.
 */

void _impl_thread_rwlock_wrunlock(struct thread_rwlock_t *lock)
{
	int err;

	err = pthread_rwlock_unlock(&lock->rwlock);
	if(err != 0)
		throw("Failed to unlock the read-write lock for writing. %s.", strerror(err));
}
