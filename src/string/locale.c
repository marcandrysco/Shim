#include "../common.h"
#include "locale.h"


/**
 * Determine if a character is a digit.
 *   @ch: The character.
 *   &returns: True if digit, false otherwise.
 */

_export
bool str_isdigit(char ch)
{
	return (ch >= '0') && (ch <= '9');
}

/**
 * Determine if a character is whitespace.
 *   @ch: The character.
 *   &returns: True if whitespace, false otherwise.
 */

_export
bool str_isspace(char ch)
{
	return (ch == ' ') || (ch == '\t') || (ch == '\v') || (ch == '\n');
}
