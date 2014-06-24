#ifndef IO_ITER_H
#define IO_ITER_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * structure prototypes
 */

struct io_chunk_t;
struct io_filter_t;


/**
 * I/O iterator function.
 *   @ref: The reference.
 *   &returns: The chunk.
 */

typedef struct io_chunk_t (*io_iter_f)(void *ref);

/**
 * Iterator interface structure.
 *   @next: The next function.
 *   @delete: The internal deletion function.
 */

struct io_iter_i {
	io_iter_f next;
	delete_f delete;
};

/**
 * Iteator instance storage structure.
 *   @ref: The internal reference.
 *   @iface: The iterator interface.
 */

struct io_iter_t {
	void *ref;
	const struct io_iter_i *iface;
};


/*
 * i/o iterator function declarations
 */

struct io_iter_t io_iter_filter(struct iter_t inner, struct io_filter_t apply);


/**
 * Obtain the next chunk from the iterator.
 *   @iter: The iterator.
 *   &returns: The reference. 'NULL' if at the end of the iterator.
 */

static inline struct io_chunk_t io_iter_next(struct io_iter_t iter)
{
	return iter.iface->next(iter.ref);
}

/**
 * Delete the iterator.
 *   @iter: The iterator.
 */

static inline void io_iter_delete(struct io_iter_t iter)
{
	iter.iface->delete(iter.ref);
}

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
