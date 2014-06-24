#include "../common.h"
#include "iter.h"
#include "../mem/manage.h"
#include "../types/iter.h"
#include "chunk.h"
#include "filter.h"


/**
 * Filter structure.
 *   @inner: The inner iterator.
 *   @filter: The filter handler.
 */

struct apply_t {
	struct iter_t inner;

	struct io_filter_t filter;
};


/*
 * local function declarations
 */

static struct io_chunk_t apply_next(struct apply_t *apply);
static void apply_delete(struct apply_t *apply);


/**
 * Create a filter around an inner iterator.
 *   @inner: The inner iterator.
 *   @filter: The filter handler.
 *   &returns: The iterator.
 */

_export
struct io_iter_t io_iter_filter(struct iter_t inner, struct io_filter_t filter)
{
	struct apply_t *apply;
	static struct io_iter_i iface = { (io_iter_f)apply_next, (delete_f)apply_delete };

	apply = mem_alloc(sizeof(struct apply_t));
	*apply = (struct apply_t){ inner, filter };

	return (struct io_iter_t){ apply, &iface };
}

/**
 * Retrieve the next reference from the filter.
 *   @apply: The filter.
 *   &returns: The reference.
 */

static struct io_chunk_t apply_next(struct apply_t *apply)
{
	void *ref;
	struct io_chunk_t chunk;

	while((ref = iter_next(apply->inner)) != NULL) {
		chunk = io_filter_apply(apply->filter, ref);
		if(!io_chunk_isnull(chunk))
			return chunk;
	}

	return io_chunk_null;
}

/**
 * Delete a iterator filter.
 *   @filter: THe filter.
 */

static void apply_delete(struct apply_t *apply)
{
	iter_delete(apply->inner);
	mem_free(apply);
}
