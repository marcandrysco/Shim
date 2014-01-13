#include "../common.h"
#include "base.h"
#include <string.h>


/**
 * Determine the length of a string.
 *   @str: The string.
 *   &returns: The length in bytes.
 */

_export
size_t str_len(const char *str)
{
	const char *orig = str;

	while(*str != '\0')
		str++;

	return str - orig;
}


/**
 * Copy a string.
 *   @dest: The destination string.
 *   @src: The source string.
 */

_export
void str_copy(char *restrict dest, const char *restrict src)
{
	strcpy(dest, src);
}


/**
 * Determine if two strings are equal.
 *   @s1: The first string.
 *   @s2: The second string.
 *   &returns: True if they are equal, false otherwise.
 */

_export
bool str_isequal(const char *s1, const char *s2)
{
	return strcmp(s1, s2) == 0;
}

/**
 * Compare two string.
 *   @s1: The first string.
 *   @s2: The second string.
 *   &returns: Their order.
 */

_export
int str_cmp(const char *s1, const char *s2)
{
	return strcmp(s1, s2);
}
