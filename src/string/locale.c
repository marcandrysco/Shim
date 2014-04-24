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

/**
 * Determine if a character is an alphabetical character.
 *   @ch: The character.
 *   &returns: True if alphabetical, false otherwise.
 */

_export
bool str_isalpha(char ch)
{
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

/**
 * Determine if a character is an alphabetical or numerical character.
 *   @ch: The character.
 *   &returns: True if alphanumerical, false otherwise.
 */

_export
bool str_isalnum(char ch)
{
	return str_isalpha(ch) || str_isdigit(ch);
}
