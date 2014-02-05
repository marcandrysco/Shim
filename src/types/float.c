#include "../common.h"
#include "float.h"
#include "../mem/manage.h"


typedef uint64_t word_t;
typedef uint32_t half_t;


/**
 * Floating point value structure.
 *   @exp: The exponent.
 *   @len: The legnth of the frection.
 *   @frac: The fractional value.
 */

struct float_t {
	int32_t exp;

	unsigned int len;
	word_t frac[];
};


/**
 * Create a new float.
 *   &returns: The float.
 */

_export
struct float_t *float_new()
{
	struct float_t *value;

	value = mem_alloc(sizeof(struct float_t));

	return value;
}

/**
 * Delete a float.
 *   @value: The float.
 */

_export
void float_delete(struct float_t *value)
{
	mem_free(value);
}
