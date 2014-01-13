#include "../common.h"
#include "locale.h"


/**
 * Determine if a character is a digit.
 *   @ch: The character.
 *   &returns: True if digit, false otherwise.
 */

bool str_isdigit(char ch)
{
	return (ch >= '0') && (ch <= '9');
}
