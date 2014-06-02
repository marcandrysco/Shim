#include "../common.h"
#include "iter.h"
#include "../mem/manage.h"
#include "../types/iter.h"
#include "chunk.h"


/**
 * Filter structure.
 *   @inner: The inner iterator.
 *   @handler: The filter handler.
 */

struct filter_t {
	struct iter_t inner;

	struct io_filter_h handler;
};


/*
 * local function declarations
 */

static struct io_chunk_t filter_next(struct filter_t *filter);
static void filter_delete(struct filter_t *filter);


/**
 * Create a filter around an inner iterator.
 *   @inner: The inner iterator.
 *   @filter: The filter handler.
 *   &returns: The iterator.
 */

_export
struct io_iter_t io_iter_filter(struct iter_t inner, struct io_filter_h handler)
{
	struct filter_t *filter;
	static struct io_iter_i iface = { (io_iter_f)filter_next, (delete_f)filter_delete };

	filter = mem_alloc(sizeof(struct filter_t));
	*filter = (struct filter_t){ inner, handler };

	return (struct io_iter_t){ filter, &iface };
}

/**
 * Retrieve the next reference from the filter.
 *   @filter: The filter.
 *   &returns: The reference.
 */

static struct io_chunk_t filter_next(struct filter_t *filter)
{
	void *ref;
	struct io_chunk_t chunk;

	while((ref = iter_next(filter->inner)) != NULL) {
		chunk = io_filter_apply(filter->handler, ref);
		if(!io_chunk_isnull(chunk))
			return chunk;
	}

	return io_chunk_null;
}

/**
 * Delete a iterator filter.
 *   @filter: THe filter.
 */

static void filter_delete(struct filter_t *filter)
{
	iter_delete(filter->inner);
	mem_free(filter);
}
