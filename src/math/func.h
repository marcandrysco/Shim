#ifndef MATH_FUNC_H
#define MATH_FUNC_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Unsigned 16-bit integer maximum.
 *   @a: The first parameter.
 *   @b: The second peramater.
 *   &returns: The maximum.
 */

static inline uint16_t m_uint16max(uint16_t a, uint16_t b)
{
	return (a > b) ? a : b;
}

/**
 * Unsigned 32-bit integer maximum.
 *   @a: The first parameter.
 *   @b: The second peramater.
 *   &returns: The maximum.
 */

static inline uint32_t m_uint32max(uint32_t a, uint32_t b)
{
	return (a > b) ? a : b;
}

/**
 * Unsigned integer minimum.
 *   @a: The first parameter.
 *   @b: The second peramater.
 *   &returns: The minimum.
 */

static inline unsigned int m_uintmin(unsigned int a, unsigned int b)
{
	return (a < b) ? a : b;
}

/**
 * Unsigned integer maximum.
 *   @a: The first parameter.
 *   @b: The second peramater.
 *   &returns: The maximum.
 */

static inline unsigned int m_uintmax(unsigned int a, unsigned int b)
{
	return (a > b) ? a : b;
}

/**
 * Unsigned 32-bit integer maximum.
 *   @a: The first parameter.
 *   @b: The second peramater.
 *   &returns: The maximum.
 */

static inline size_t m_sizemax(size_t a, size_t b)
{
	return (a > b) ? a : b;
}


/**
 * Unsigned 32-bit integer minimum.
 *   @a: The first parameter.
 *   @b: The second peramater.
 *   &returns: The minimum.
 */

static inline size_t m_sizemin(size_t a, size_t b)
{
	return (a < b) ? a : b;
}


/**
 * Double maximum.
 *   @left: The left value.
 *   @right: The right value.
 *   &returns: The maximum.
 */

static inline double m_dblmax(double a, double b)
{
	return (a > b) ? a : b;
}

/**
 * Double minimum.
 *   @left: The left value.
 *   @right: The right value.
 *   &returns: The minimum.
 */

static inline double m_dblmin(double a, double b)
{
	return (a < b) ? a : b;
}

/**
 * Double modulus operator.
 *   @a: The input value.
 *   @b: The modulus.
 *   &returns: The modulated value.
 */

static inline double m_dblmod(double a, double b)
{
	return fmod(a, b);
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
