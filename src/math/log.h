#ifndef MATH_LOG_H
#define MATHLOGC_H

/*
 * start header: shim.h
 */

/* %shim.h% */


static inline double m_round(double val, double mul)
{
	return mul * round(val / mul);
}

static inline double m_log10round(double val, unsigned int n)
{
	double t;

	t = pow(10, ceil(log10(val / (5.5 / n))));
	return m_round(val, t / n) ?: t / 10.0;
}

static inline double log10_prev(double val, unsigned int n)
{
	double inc, t;

	val = log10_rnd(val, n);
	t = pow(10, ceil(log10(val)));
	inc = t / n;
	return m_round(val - inc, inc) ?: t / 10.0;
}

static inline double log10_next(double val, unsigned int n)
{
	double inc, t;

	val = log10_rnd(val, n);
	t = pow(10, floor(log10(val)) + 1.0);
	inc = t / n;
	return m_round(val + inc - inc / 4, inc) ?: t / 10.0;
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
