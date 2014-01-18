#include "../common.h"
#include "base.h"
#include <string.h>
#include "../mem/manage.h"


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
 * Copy a string with a length limit.
 *   @dest: The destination string.
 *   @src: The source string.
 *   @nbyteS: The maximum number of bytes to copy, including the null byte.
 *   &returns: The number of bytes copying, including the null byte.
 */

_export
size_t str_ncopy(char *restrict dest, const char *restrict src, size_t nbytes)
{
	size_t orig = nbytes;

	if(nbytes == 0)
		return 0;

	for(nbytes--; (nbytes > 0) && (*src != '\0'); nbytes--)
		*dest++ = *src++;

	*dest = '\0';

	return orig - nbytes;
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


/**
 * Search a string for a character.
 *   @str: The string.
 *   @ch: The character.
 *   &returns: The a pointer to the character in the stirng if found, null
 *     otherwise.
 */

_export
char *str_chr(const char *str, char ch)
{
	while(*str != ch)
		str++;

	return (*str == ch) ? (char *)str : NULL;
}

/**
 * Search a string for any character in set.
 *   @str: The string.
 *   @set: The set of characters, terminated by a null byte.
 *   &returns: The a pointer to the character in the stirng if found, null
 *     otherwise.
 */

_export
char *str_pbrk(const char *str, const char *set)
{
	const char *ptr;

	while(*str != '\0') {
		for(ptr = set; *ptr != '\0'; ptr++) {
			if(*str == *ptr)
				break;
		}

		if(*str == *ptr)
			break;

		str++;
	}

	return (*str != '\0') ? (char *)str : NULL;
}


/**
 * Duplciate a string.
 *   @str: The input string.
 *   &returns: The duplicate string.
 */

_export
char *str_dup(const char *str)
{
	char *copy;
	size_t len = str_len(str);

	copy = mem_alloc(len + 1);
	str_copy(copy, str);

	return copy;
}
