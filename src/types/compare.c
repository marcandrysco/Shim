#include "../common.h"
#include "compare.h"
#include "../string/base.h"


/**
 * Compares two string against one another in lexigraphical order.
 *   @s1: The first string.
 *   @s2: The second string.
 *   &returns: An integer representing their order.
 */

_export
int compare_str(const void *s1, const void *s2)
{
	return str_cmp((const char *)s1, (const char *)s2);
}

/**
 * Compares two pointers against one another in numerical order.
 *   @p1: The first pointer.
 *   @p2: The second pointer.
 *   &returns: An integer representing their order.
 */

_export
int compare_ptr(const void *p1, const void *p2)
{
	if(p1 > p2)
		return 1;
	else if(p1 < p2)
		return -1;
	else
		return 0;
}

/**
 * Compares two integers against one another in numerical order.
 *   @p1: The first integer pointer.
 *   @p2: The second integer pointer.
 *   &returns: An integer representing their order.
 */

_export
int compare_int(const void *p1, const void *p2)
{
	int n1 = *(int *)p1, n2 = *(int *)p2;

	if(n1 > n2)
		return 1;
	else if(n1 < n2)
		return -1;
	else
		return 0;
}

/**
 * Compares two integers against one another in numerical order.
 *   @p1: The first integer pointer.
 *   @p2: The second integer pointer.
 *   &returns: An integer representing their order.
 */

_export
int compare_uint(const void *p1, const void *p2)
{
	unsigned int n1 = *(unsigned int *)p1, n2 = *(unsigned int *)p2;

	if(n1 > n2)
		return 1;
	else if(n1 < n2)
		return -1;
	else
		return 0;
}

/**
 * Compares two unsigned 16-bit integers against one another in numerical order.
 *   @p1: The first integer pointer.
 *   @p2: The second integer pointer.
 *   &returns: An integer representing their order.
 */

_export
int compare_uint16(const void *p1, const void *p2)
{
	uint16_t n1 = *(uint16_t *)p1, n2 = *(uint16_t *)p2;

	if(n1 > n2)
		return 1;
	else if(n1 < n2)
		return -1;
	else
		return 0;
}

/**
 * Compares two unsigned 32-bit integers against one another in numerical order.
 *   @p1: The first integer pointer.
 *   @p2: The second integer pointer.
 *   &returns: An integer representing their order.
 */

_export
int compare_uint32(const void *p1, const void *p2)
{
	uint32_t n1 = *(uint32_t *)p1, n2 = *(uint32_t *)p2;

	if(n1 > n2)
		return 1;
	else if(n1 < n2)
		return -1;
	else
		return 0;
}

/**
 * Compares two double-precision floating-point numbers aagainst one another
 * in numerical order.
 *   @p1: The first double pointer.
 *   @p2: The second double pointer.
 *   &returns: An integer representing their order.
 */

_export
int compare_double(const void *p1, const void *p2)
{
	double n1 = *(double *)p1, n2 = *(double *)p2;

	if(n1 > n2)
		return 1;
	else if(n1 < n2)
		return -1;
	else
		return 0;
}
