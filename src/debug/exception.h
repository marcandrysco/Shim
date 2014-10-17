#ifndef DEBUG_EXCEPTION_H
#define DEBUG_EXCEPTION_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * setjmp header
 */

#include <setjmp.h>

/*
 * fatal function declarations
 */

void _nothrow();
_noreturn void _abort();
_noreturn void _shim_fatal(const char *file, unsigned int line, const char *restrict format, ...);

void _backtrace();


/**
 * Try state structure.
 *   @fatal: Fatal flag.
 *   @jmpbuf: The jump buffer.
 *   @prev: The previous try on the stack.
 */

struct _shim_try_t {
	bool fatal;
	jmp_buf jmpbuf;

	struct _shim_try_t *prev;
};


/*
 * exception function declarations
 */

struct _shim_try_t *_shim_try_start();
void _shim_try_end(struct _shim_try_t *inst);

const char *_shim_catch_start();
void _shim_catch_end();

_noreturn void _shim_throw(const char *file, unsigned int line, const char *restrict format, ...);
_noreturn void _shim_vthrow(const char *file, unsigned int line, const char *restrict format, va_list args);
_noreturn void _shim_sthrow(const char *file, unsigned int line, char *restrict error);

#define try		for(struct _shim_try_t *_shim_try = _shim_try_start(); _shim_try != NULL; _shim_try_end(_shim_try), _shim_try = NULL) if(!setjmp(_shim_try->jmpbuf)) switch(0) case 0:
#define catch(e)	else for(const char *e = (_shim_try = NULL, _shim_catch_start()); e != NULL ; _shim_catch_end(), e = NULL) switch(0) case 0:

#define _fatal(...)	_shim_fatal(__FILE__, __LINE__, __VA_ARGS__)
#define throw(...)	_shim_throw(__FILE__, __LINE__, __VA_ARGS__)
#define vthrow(...)	_shim_vthrow(__FILE__, __LINE__, __VA_ARGS__)

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
