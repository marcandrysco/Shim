#ifndef MATH_RAND_H
#define MATH_RAND_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Random storage structure.
 *   @x, y, z, w: Storage data.
 */

struct m_rand_t {
	uint32_t x, y, z, w;
};


/**
 * Create a new random generator.
 *   @seed: The seed.
 *   &returns; The random generator.
 */

static inline struct m_rand_t m_rand_new(uint32_t seed)
{
	return (struct m_rand_t){ 123456789, 362436069, 521288629, seed };
}


/*
 * random function declarations
 */

uint32_t m_rand_next(struct m_rand_t *rand);
double m_rand_nextd(struct m_rand_t *rand);

/*
 * random function declarations
 */

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
