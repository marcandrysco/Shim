#ifndef IO_CHUNK_H
#define IO_CHUNK_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Chunked output callback.
 *   @output: The output device.
 *   @arg: The argument.
 */

typedef void (*io_chunk_f)(struct io_output_t output, void *arg);

/**
 * Chunked output structure.
 *   @func: The callback function.
 *   @arg: The callback argument.
 */

struct io_chunk_t {
	io_chunk_f func;
	void *arg;
};


/*
 * chunk variables
 */

extern struct io_chunk_t io_chunk_null;

/*
 * chunk function declarations
 */

void io_chunk_proc(struct io_chunk_t chunk, struct io_output_t output);
char *io_chunk_proc_str(struct io_chunk_t chunk);
size_t io_chunk_proc_len(struct io_chunk_t chunk);

struct io_chunk_t io_chunk_str(const char *str);
struct io_chunk_t io_chunk_strptr(char **str);
struct io_chunk_t io_chunk_cond(const struct io_chunk_t *pair);
struct io_chunk_t io_chunk_indent(const unsigned int *indent);

/*
 * chunk definitions
 */

#define io_chunk_nullstr(chunk, str) io_chunk_cond((struct io_chunk_t []){ chunk, io_chunk_str(str) })
#define io_chunk_indentval(val) io_chunk_indent(&(union { unsigned int i; }){ .i = val }.i)


/**
 * Determine if a chunk is null.
 *   @chunk: The chunk.
 *   &returns: True if null, false otherwise.
 */

static inline bool io_chunk_isnull(struct io_chunk_t chunk)
{
	return (chunk.func == NULL) && (chunk.arg == NULL);
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
