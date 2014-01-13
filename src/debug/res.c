#include "../common.h"
#include "res.h"
#include "../io/chunk.h"
#include "../io/print.h"
#include "../io/output.h"
#include "../mem/base.h"
#include "../string/base.h"
#include "../types/avltree.h"
#include "../types/compare.h"
#include "../types/compare.h"
#include "exception.h"


/**
 * Resource reference structure.
 *   @node: The tree node.
 *   @res: The resource.
 *   @file: The file.
 *   @line: The line.
 *   @info: Resource information.
 *   @backtrace: Backtrace information.
 */

struct ref_t {
	struct avltree_node_t node;

	void *res;

	char *file;
	unsigned int line;

	char *info;
	void *backtrace[4];
};


/*
 * implementation function declarations
 */

void *_impl_mem_alloc(size_t nbytes);
void *_impl_mem_realloc(void *ptr, size_t nbytes);
void _impl_mem_free(void *ptr);

void _impl_sys_atexit_init();

/*
 * local function declarations
 */

static size_t info_write(void *ref, const void *restrict buf, size_t len);

static int compare_key(const void *key, const struct avltree_node_t *node, void *arg);
static int compare_node(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg);

/*
 * local variables
 */

static struct avltree_node_t *res_tree = NULL;
static unsigned int res_count = 0;

static struct io_output_i info_iface = { { NULL, NULL }, (io_write_f)info_write };


/**
 * Allocate a resource.
 *   @res: The resource.
 *   @file: The file name.
 *   @line: The line.
 *   @info: Optional. Resource information.
 */

_export
void _dbg_res_alloc(void *res, const char *file, unsigned int line, struct io_chunk_t info)
{
	struct ref_t *ref;

	_impl_sys_atexit_init();

	ref = _impl_mem_alloc(sizeof(struct ref_t));
	ref->res = res;
	ref->line = line;
	ref->file = _impl_mem_alloc(str_len(file) + 1);
	str_copy(ref->file, file);

	if(!io_chunk_isnull(info)) {
		ref->info = _impl_mem_alloc(1);
		ref->info[0] = '\0';
		io_chunk_proc(info, (struct io_output_t){ &ref->info, &info_iface });
	}
	else
		ref->info = NULL;

	res_count++;
	avltree_node_insert(&res_tree, &ref->node, compare_node, NULL);
}

/**
 * Free a resource.
 *   @res: The resource.
 */

_export
void _dbg_res_free(void *res)
{
	struct ref_t *ref;

	_impl_sys_atexit_init();

	ref = (struct ref_t *)avltree_node_remove(&res_tree, res, compare_key, NULL);
	if(ref == NULL)
		_fatal("Invalid free.");

	res_count--;

	if(ref->info != NULL)
		_impl_mem_free(ref->info);

	_impl_mem_free(ref->file);
	_impl_mem_free(ref);
}


/**
 * Retrieve the current number of allocated resources.
 *   &returns: The number of still allocated resources.
 */

_export
unsigned int dbg_res_count()
{
	return res_count;
}

/**
 * Dump the current used resources.
 *   @output: The output.
 */

_export
void dbg_res_dump(struct io_output_t output)
{
	struct ref_t *ref;
	struct avltree_iter_t iter;

	avltree_node_iter_init(&iter, res_tree);
	while((ref = (struct ref_t *)avltree_node_iter_next(&iter)) != NULL) {
		io_printf(output, "%s:%u", ref->file, ref->line);

		if(ref->info != NULL)
			io_printf(output, " - %s", ref->info);

		io_printf(output, "\n");
	}
}

/**
 * Exit handler for printing unfreed resources.
 */

void _dbg_res_atexit()
{
	unsigned int count = dbg_res_count();

	if(count == 0)
		return;
	
	io_printf(io_stderr, "Warning: Unfreed resources (%u).\n", count);
	io_printf(io_stderr, "--begin dump--\n");
	dbg_res_dump(io_stderr);
}


/**
 * Write data into the accumulated buffer.
 *   @ref: The buffer reference.
 *   @buf: The data written.
 *   @len: The number of bytes to write.
 *   &returns: The number of bytes written.
 */

static size_t info_write(void *ref, const void *restrict buf, size_t len)
{
	char **info = ref;
	unsigned int prev = str_len(*info);

	*info = _impl_mem_realloc(*info, prev + len + 1);
	mem_copy(*info + prev, buf, len);
	(*info)[prev + len] = '\0';

	return len;
}


/**
 * Comparison callback between a node and a key.
 *   @key: The input key.
 *   @node: The compared-to node.
 *   @arg: The callback argument.
 *   &returns: An integer representing their order.
 */

static int compare_key(const void *key, const struct avltree_node_t *node, void *arg)
{
	const struct ref_t *ref = (const struct ref_t *)node;

	return compare_ptr(key, ref->res);
}

/**
 * Comparison callback between two nodes.
 *   @n1: The first node.
 *   @n2: The seconde node.
 *   @arg: The callback argument.
 *   &returns: An integer representing their order.
 */

static int compare_node(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg)
{
	const struct ref_t *ref1 = (const struct ref_t *)n1, *ref2 = (const struct ref_t *)n2;

	return compare_ptr(ref1->res, ref2->res);
}
