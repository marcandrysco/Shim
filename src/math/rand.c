#include "../common.h"
#include "rand.h"


/**
 * Retrieve the next number from the generator.
 *   @rand: The number generator.
 *   &returns: The next number.
 */

_export
uint32_t m_rand_next(struct m_rand_t *rand)
{
	  uint32_t t;

	  t = rand->x ^ (rand->x << 11);
	  rand->x = rand->y;
	  rand->y = rand->z;
	  rand->z = rand->w;
	  rand->w = rand->w ^ (rand->w >> 19) ^ (t ^ (t >> 8));
	  return rand->w;
}
