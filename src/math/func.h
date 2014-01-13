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
 */

static inline uint16_t m_uint16max(uint16_t a, uint16_t b)
{
	return (a > b) ? a : b;
}

/**
 * Unsigned 32-bit integer maximum.
 *   @a: The first parameter.
 *   @b: The second peramater.
 */

static inline uint32_t m_uint32max(uint32_t a, uint32_t b)
{
	return (a > b) ? a : b;
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
