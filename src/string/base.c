#include "../common.h"
#include "base.h"
#include <string.h>
#include "../mem/manage.h"
#include "locale.h"


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
	while((*str != '\0') && (*str != ch))
		str++;

	return (*str == ch) ? (char *)str : NULL;
}

/**
 * Search a string for a character in reverse.
 *   @str: The string.
 *   @ch: The character.
 *   &returns: The a pointer to the character in the stirng if found, null
 *     otherwise.
 */

_export
char *str_rchr(const char *str, char ch)
{
	const char *orig = str;

	str += str_len(str);

	while((str >= orig) && (*str != ch))
		str--;

	return (str >= orig) ? (char *)str : NULL;
}

/**
 * Search a string for a substring.
 *   @str: The string.
 *   @sub: The substring.
 *   &returns: The a pointer to the character in the stirng if found, null
 *     otherwise.
 */

_export
char *str_str(const char *str, char *sub)
{
	return strstr(str, sub);
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
 * Concatonate a string, reallocating space for the final string.
 *   @base: The base string reference.
 *   @str: The string to concatonate.
 */

_export
void str_cat(char **restrict base, const char *restrict str)
{
	size_t len[2] = { str_len(*base), str_len(str) };

	*base = mem_realloc(*base, len[0] + len[1] + 1);
	str_copy(*base + len[0], str);
}

/**
 * Duplicate a string.
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

/**
 * Duplicate a string, copying at most 'n' characters from the input..
 *   @str: The input string.
 *   @n: The max length in characters.
 *   &returns: The duplicate string.
 */

_export
char *str_ndup(const char *str, size_t n)
{
	char *copy;
	size_t len = str_len(str);

	if(len > n)
		len = n;

	copy = mem_alloc(len + 1);
	str_ncopy(copy, str, len + 1);

	return copy;
}

/**
 * Clone a string, allocating the required space if it is non-null.
 *   @str: The string, may be null.
 *   &returns: Duplicated string, or null if input is null.
 */

_export
char *str_clone(const char *str)
{
	return str ? str_dup(str) : NULL;
}

/**
 * Replace the string in destination with source. Either string may be null.
 *   @dest: The destination.
 *   @src: The source.
 */

_export
void str_replace(char **dest, const char *src)
{
	if(*dest != NULL)
		mem_free(*dest);

	*dest = src ? str_dup(src) : NULL;
}

/**
 * Trim whitespace off the right side of the string. The string is reallocated
 * fit exactly.
 *   @str: The string reference.
 */

_export
void str_rtrim(char **str)
{
	char *endptr = *str + str_len(*str) - 1;

	while((endptr >= *str) && str_isspace(*endptr))
		endptr--;

	*(++endptr) = '\0';
	*str = mem_realloc(*str, endptr - *str + 1);
}
