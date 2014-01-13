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


/**
 * Object output callback.
 *   @ref: the object reference.
 *   @output: The output device.
 */

typedef void (*io_object_f)(void *ref, struct io_output_t output);

/**
 * Object interface.
 *   @proc: Process output.
 *   @close: Close object.
 */

struct io_object_i {
	io_object_f proc;
	io_close_f close;
};

/**
 * Output object structure.
 *   @ref: The reference.
 *   @iface: The interface.
 */

struct io_object_t {
	void *ref;
	const struct io_object_i *iface;
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

struct io_chunk_t io_chunk_str(const char *str);

/*
 * object function declarations
 */

void io_object_proc(struct io_object_t object, struct io_output_t output);
void io_object_close(struct io_object_t object);


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
