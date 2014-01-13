#include "../../common.h"
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../../debug/exception.h"


/**
 * Retrieve the time in seconds.
 *   &returns: The number of seconds since 1970.
 */

int64_t _impl_sys_time()
{
	return time(NULL);
}

/**
 * Retrieve the time in microseconds.
 *   &returns: The number of microseconds since 1970.
 */

int64_t _impl_sys_utime()
{
	struct timeval tv;

	if(gettimeofday(&tv, NULL) < 0)
		throw("Failed to get current time. %s.", strerror(errno));

	return 1000000 * (int64_t)tv.tv_sec + (int64_t)tv.tv_usec;
}


/**
 * Sleep an interval given in seocnds.
 *   @sec: The number of seconds to sleep.
 */

void _impl_sys_sleep(uint64_t sec)
{
	sleep(sec);
}

/**
 * Sleep an interval given in microseocnds.
 *   @usec: The number of microseconds to sleep.
 */

void _impl_sys_usleep(uint64_t usec)
{
	struct timespec rem, req = { usec / 1000000, (usec % 1000000) * 1000 };

	while(nanosleep(&req, &rem) < 0) {
		if(errno != EINTR)
			throw("Failed to sleep. %s.", strerror(errno));

		req = rem;
	}
}
