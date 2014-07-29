#ifndef IO_FILTER_H
#define IO_FILTER_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Filter an input element to a chunk.
 *   @ref: The filter reference.
 *   @in: The input.
 *   &returns: The output chunk.
 */

typedef struct io_chunk_t (*io_filter_f)(void *ref, void *in);

/**
 * Filter an input element.
 *   @in: The input.
 *   @arg: The argument.
 *   &returns: The output chunk.
 */

typedef struct io_chunk_t (*io_filter_apply_f)(void *in, void *arg);

/**
 * Filter interface structure.
 *   @apply: The filter application function.
 *   @delete: The internal deletion function.
 */

struct io_filter_i {
	io_filter_f apply;
	delete_f delete;
};

/**
 * Enumerator storage structure.
 *   @ref: The internal reference.
 *   @iface: The filter interface.
 */

struct io_filter_t {
	void *ref;
	const struct io_filter_i *iface;
};


/*
 * filter variables
 */

extern struct io_filter_t io_filter_blank;
extern struct io_filter_t io_filter_str;

/*
 * filter function declarations
 */

struct io_filter_t io_filter_new(io_filter_apply_f func, void *arg);


/**
 * Create the iterator from a filter.
 *   @filter: The filter.
 *   @in: The input.
 *   &returns: The chunk.
 */

static inline struct io_chunk_t io_filter_apply(struct io_filter_t filter, void *in)
{
	return filter.iface->apply(filter.ref, in);
}

/**
 * Delete the filter.
 *   @filter: The filter.
 */

static inline void io_filter_delete(struct io_filter_t filter)
{
	filter.iface->delete(filter.ref);
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
