#include "../common.h"
#include "enum.h"
#include "../mem/manage.h"
#include "../types/avltree.h"
#include "filter.h"
#include "iter.h"


/**
 * Instance structure.
 *   @ref: The reference.
 *   @iter: the iterator callback.
 *   @delete: The delete callback.
 */

struct inst_t {
	void *ref;
	enum_f iter;
	delete_f delete;
};

/**
 * Apply structure for enumeration.
 *   @inner: The inner enumerator.
 *   @filter: The filter.
 */

struct apply_t {
	struct enum_t inner;
	struct filter_t filter;
};

/**
 * Sort structure for enumeration.
 *   @inner: The inner enumerator.
 *   @compare: The comparison callback.
 */

struct sort_t {
	struct enum_t inner;
	compare_f compare;
};

/**
 * Compose structure for enumeration.
 *   @outer, inner: The outer and inner enumerators.
 */

struct compose_t {
	struct enum_t outer;
	enum_f inner;
};


/*
 * local function declarations
 */

static struct iter_t blank_iter(void *ref);

static struct iter_t inst_iter(struct inst_t *inst);
static void inst_delete(struct inst_t *inst);

static struct iter_t apply_iter(struct apply_t *apply);
static void apply_delete(struct apply_t *apply);

static struct iter_t sort_iter(struct sort_t *sort);
static void sort_delete(struct sort_t *sort);

static struct iter_t compose_iter(struct compose_t *compose);
static void compose_delete(struct compose_t *compose);

/*
 * local variables
 */

static struct enum_i blank_iface = { blank_iter, delete_noop };

/*
 * global variables
 */

_export struct enum_t enum_blank = { NULL, &blank_iface };


/**
 * Generaete a blank iterator.
 *   @ref: Unusued reference.
 */

static struct iter_t blank_iter(void *ref)
{
	return iter_blank;
}


/**
 * Create a new enumerator from a function and argument.
 *   @ref: The reference.
 *   @iter: The iterator callback.
 *   @delete: The deletion callback.
 *   &returns: The enumerator.
 */

_export
struct enum_t enum_new(void *ref, enum_f iter, delete_f delete)
{
	struct inst_t *inst;
	static struct enum_i iface = { (enum_f)inst_iter, (delete_f)inst_delete };

	inst = mem_alloc(sizeof(struct inst_t));
	*inst = (struct inst_t){ ref, iter, delete };

	return (struct enum_t){ inst, &iface };
}

/**
 * Creata an iterator from the enumerator instance.
 *   @inst: The instance.
 *   &returns: The iterator.
 */

static struct iter_t inst_iter(struct inst_t *inst)
{
	return inst->iter(inst->ref);
}

/**
 * Delete an enumerator instance.
 *   @inst: The instance.
 */

static void inst_delete(struct inst_t *inst)
{
	inst->delete(inst->ref);
	mem_free(inst);
}


/**
 * Create an enumerator that will apply an inner filter.
 *   @inner: The inner enumerator.
 *   @filter: The filter.
 *   &returns: The enumereator.
 */

_export
struct enum_t enum_filter(struct enum_t inner, struct filter_t filter)
{
	struct apply_t *apply;
	static struct enum_i iface = { (enum_f)apply_iter, (delete_f)apply_delete };

	apply = mem_alloc(sizeof(struct apply_t));
	*apply = (struct apply_t){ inner, filter };

	return (struct enum_t){ apply, &iface };
}

/**
 * Create an iterator from the apply enumerator.
 *   @apply: The apply structure.
 *   &returns: The iterator.
 */

static struct iter_t apply_iter(struct apply_t *apply)
{
	return iter_filter(enum_iter(apply->inner), apply->filter);
}

/**
 * Delete an apply enumerator.
 *   @apply: The apply structure.
 */

static void apply_delete(struct apply_t *apply)
{
	enum_delete(apply->inner);
	filter_delete(apply->filter);
	mem_free(apply);
}


/**
 * Create an enumerator that sorts an input enumerator.
 *   @inner: The inner enumerator.
 *   @compare: The comparison callback.
 *   &returns: The enumereator.
 */

_export
struct enum_t enum_sort(struct enum_t inner, compare_f compare)
{
	struct sort_t *sort;
	static struct enum_i iface = { (enum_f)sort_iter, (delete_f)sort_delete };

	sort = mem_alloc(sizeof(struct sort_t));
	*sort = (struct sort_t){ inner, compare };

	return (struct enum_t){ sort, &iface };
}

/**
 * Create an iterator from the sort enumerator.
 *   @sort: The sort structure.
 *   &returns: The iterator.
 */

static struct iter_t sort_iter(struct sort_t *sort)
{
	void *ref;
	struct iter_t iter;
	struct avltree_t *tree;

	tree = avltree_new(sort->compare, NULL);

	iter = enum_iter(sort->inner);
	while((ref = iter_next(iter)) != NULL)
		avltree_insert(tree, ref, ref);

	iter_delete(iter);

	return avltree_asiter(tree);
}

/**
 * Delete an sort enumerator.
 *   @sort: The sort structure.
 */

static void sort_delete(struct sort_t *sort)
{
	enum_delete(sort->inner);
	mem_free(sort);
}


/**
 * Compose two enumerators.
 *   @outer: The outer enumerator.
 *   @inner: the inner enumerator callback.
 *   &returns: The enumerator.
 */

_export
struct enum_t enum_compose(struct enum_t outer, enum_f inner)
{
	struct compose_t *compose;
	static struct enum_i iface = { (enum_f)compose_iter, (delete_f)compose_delete };

	compose = mem_alloc(sizeof(struct compose_t));
	*compose = (struct compose_t){ outer, inner };

	return (struct enum_t){ compose, &iface };
}

/**
 * Retrieve the iterator from a composed enumerator.
 *   @compose: The composed enumerator.
 *   &returns: The iterator.
 */

static struct iter_t compose_iter(struct compose_t *compose)
{
	return iter_compose(enum_iter(compose->outer), compose->inner);
}

/**
 * Delete a composed enumerator.
 *   @compose: The composed enumerator.
 */

static void compose_delete(struct compose_t *compose)
{
	enum_delete(compose->outer);
	mem_free(compose);
}
