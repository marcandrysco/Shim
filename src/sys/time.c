#include "../common.h"


/*
 * implementation function declarations
 */

int64_t _impl_sys_time();
int64_t _impl_sys_utime();

void _impl_sys_sleep(uint64_t sec);
void _impl_sys_usleep(uint64_t usec);


/**
 * Retrieve the time in seconds.
 *   &returns: The number of seconds since 1970.
 */

_export
int64_t sys_time()
{
	return _impl_sys_time();
}

/**
 * Retrieve the time in microseconds.
 *   &returns: The number of microseconds since 1970.
 */

_export
int64_t sys_utime()
{
	return _impl_sys_utime();
}


/**
 * Sleep an interval given in seocnds.
 *   @sec: The number of seconds to sleep.
 */

_export
void sys_sleep(uint64_t sec)
{
	_impl_sys_sleep(sec);
}

/**
 * Sleep an interval given in microseocnds.
 *   @usec: The number of microseconds to sleep.
 */

_export
void sys_usleep(uint64_t usec)
{
	_impl_sys_usleep(usec);
}
