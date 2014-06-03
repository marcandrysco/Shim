#include "../common.h"
#include "exception.h"
#include "../io/output.h"
#include "../io/print.h"
#include "../mem/manage.h"
#include "../string/base.h"
#include "../string/io.h"
#include "../sys/proc.h"
#include "../thread/base.h"
#include "../thread/local.h"

#if BMAKE__HOST_windows
#else
#	include "../thread/posix/defs.h"
#endif


/*
 * implementation function declarations
 */

_noreturn void _impl_abort();
void _impl_backtrace();

/*
 * local function declaraitons
 */

static void try_init();
static void try_destroy();

static void throw_init();
static void throw_destroy();

/*
 * local variables
 */

static struct thread_once_t try_once = THREAD_ONCE_INIT, throw_once = THREAD_ONCE_INIT;
static struct thread_local_t *try_local, *throw_local;


/**
 * Invalidate the current try statement, causing subsequent throws to be fatal
 * exceptions. This only effects the currently active try; nested and future
 * try statements are unaffected.
 */

_export
void _nothrow()
{
	struct _shim_try_t *inst;

	thread_once(&try_once, try_init);
	inst = thread_local_get(try_local);
	if(inst != NULL)
		inst->fatal = true;
}

/**
 * Abort the process
 */

_export _noreturn
void _abort()
{
	if(_debug)
		_backtrace();

	_impl_abort();
}

/**
 * Fatally close the program, resulting in an abort.
 *   @format: The printf-style format.a
 *   @...: The print-style arguments.
 */

_export _noreturn
void _fatal(const char *restrict format, ...)
{
	va_list args;

	va_start(args, format);
	io_vprintf(io_stderr, format, args);
	va_end(args);

	if(format[str_len(format) - 1] != '\n')
		io_printf(io_stderr, "\n");

	_abort();
}

/**
 * Backtrace function.
 */

_export
void _backtrace()
{
	_impl_backtrace();
}


/**
 * Start the try statement.
 *   &returns: The try state.
 */

_export
struct _shim_try_t *_shim_try_start()
{
	struct _shim_try_t *inst;

	thread_once(&try_once, try_init);

	inst = mem_alloc(sizeof(struct _shim_try_t));
	inst->fatal = false;
	inst->prev = thread_local_get(try_local);

	thread_local_set(try_local, inst);

	return inst;
}

/**
 * End the try statement.
 *   @inst: The try state.
 */

_export
void _shim_try_end(struct _shim_try_t *inst)
{
	if(inst == NULL)
		return;

	thread_local_set(try_local, inst->prev);

	mem_free(inst);
}

/**
 * Initialize the try state.
 */

static void try_init()
{
	try_local = thread_local_new(NULL);
	sys_atexit(try_destroy);
}

/**
 * Destroy the try state.
 */

static void try_destroy()
{
	thread_local_delete(try_local);
}


/**
 * Start a catch statement, deleting the try information.
 *   &returns: The exception string from the throw.
 */

_export
const char *_shim_catch_start()
{
	struct _shim_try_t *inst;

	thread_once(&try_once, try_init);
	thread_once(&throw_once, throw_init);

	inst = thread_local_get(try_local);
	thread_local_set(try_local, inst->prev);
	mem_free(inst);

	return thread_local_get(throw_local);
}

/**
 * End a catch statement, deleting throw information.
 */

_export
void _shim_catch_end()
{
	thread_once(&throw_once, throw_init);
	mem_delete(thread_local_get(throw_local));
	thread_local_set(throw_local, NULL);
}


/**
 * Throw an exception.
 *   @file: The file.
 *   @line: The line.
 *   @format: The message printf-style format.
 *   @...: The printf-style arguments.
 */

_export _noreturn
void _shim_throw(const char *file, unsigned int line, const char *restrict format, ...)
{
	char *error;
	va_list args;

	va_start(args, format);
	error = str_vaprintf(format, args);
	va_end(args);

	_shim_sthrow(file, line, error);
}

/**
 * Throw an exception with a variable arugment list.
 *   @file: The file.
 *   @line: The line.
 *   @format: The message printf-style format.
 *   @...: The printf-style variable arugment list.
 */

_export _noreturn
void _shim_vthrow(const char *file, unsigned int line, const char *restrict format, va_list args)
{
	_shim_sthrow(file, line, str_vaprintf(format, args));
}

/**
 * Throw an exception using an allocated string.
 *   @file: The file.
 *   @line: The line.
 *   @error: The allocated error string.
 */

_export _noreturn
void _shim_sthrow(const char *file, unsigned int line, char *restrict error)
{
	struct _shim_try_t *inst;

	thread_once(&try_once, try_init);
	inst = thread_local_get(try_local);
	if((inst == NULL) || (inst->fatal))
		_fatal("Unhandled exception.\n%s:%u:%s", file, line, error);

	thread_once(&throw_once, throw_init);
	mem_delete(thread_local_get(throw_local));
	thread_local_set(throw_local, error);

	longjmp(inst->jmpbuf, 1);
}


/**
 * Initialize the throw state.
 */

static void throw_init()
{
	throw_local = thread_local_new(NULL);
	thread_local_set(throw_local, NULL);
	sys_atexit(throw_destroy);
}

/**
 * Destroy the throw state.
 */

static void throw_destroy()
{
	mem_delete(thread_local_get(throw_local));
	thread_local_delete(throw_local);
}
