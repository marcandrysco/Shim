#ifndef MATH_FLOAT_H
#define MATH_FLOAT_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Double-precision floating-point absolute value.
 *   @val: The value.
 *   &returns: The absolute value.
 */

static inline double m_dblabs(double val)
{
	return fabs(val);
}


static inline double m_rnd(double val, double mul)
{
	return mul * round(val / mul);
}

static inline double m_log10rnd(double val, unsigned int n)
{
	double t;

	t = pow(10, ceil(log10(val / (5.5 / n))));
	return m_rnd(val, t / n) ?: t / 10.0;
}

static inline double m_log10rnd_prev(double val, unsigned int n)
{
	double inc, t;

	val = m_log10rnd(val, n);
	t = pow(10, ceil(log10(val)));
	inc = t / n;
	return m_rnd(val - inc, inc) ?: t / 10.0;
}

static inline double m_log10rnd_next(double val, unsigned int n)
{
	double inc, t;

	val = m_log10rnd(val, n);
	t = pow(10, floor(log10(val)) + 1.0);
	inc = t / n;
	return m_rnd(val + inc - inc / 4, inc) ?: t / 10.0;
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
