#include "../common.h"
#include "algo.h"


/**
 * Greatest common divisor computation.
 *   @a: The first number.
 *   @b: The second number.
 *   &return: The greatest common divisor.
 */

_export
unsigned int m_gcd(unsigned int a, unsigned int b)
{
	unsigned int t;

	while(a != 0) {
		t = a;
		a = b % a;
		b = t;
	}

	return b;
}

/**
 * Least common multiple computation.
 *   @a: The first number.
 *   @b: The second number.
 *   &return: The least common multiple.
 */

_export
unsigned int m_lcm(unsigned int a, unsigned int b)
{
	return (a * b) / m_gcd(a, b);
}
