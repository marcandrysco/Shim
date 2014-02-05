#ifndef DEFS_H
#define DEFS_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * debug definitions
 */

#if !defined(_debug) && defined(DEBUG)
#	define _debug 1
#endif

/*
 * common headers
 */

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>


/**
 * Variable argument list wrapper structure.
 *   @args: The internal argument list.
 */

struct arglist_t {
	va_list args;
};

/* %~shim.h% */

/*
 * end header: shim.h
 */


/*
 * system headers
 */

#ifdef BMAKE__HOST_WINDOWS
#else
#	include "thread/posix/defs.h"
#endif

#endif
