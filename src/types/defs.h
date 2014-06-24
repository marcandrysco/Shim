#ifndef TYPES_DEFS_H
#define TYPES_DEFS_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/**
 * Delete a reference.
 *   @ref: The reference.
 */

typedef void (*delete_f)(void *ref);

/**
 * Copy a reference.
 *   @ref: The source reference.
 *   &returns: The copied reference.
 */

typedef void *(*copy_f)(void *ref);


/**
 * Value comparison.
 *   @p1: The first pointer value.
 *   @p2: The second pointer value.
 *   &returns: Their order.
 */

typedef int (*compare_f)(const void *p1, const void *p2);


/**
 * Obtain the next iterator reference.
 *   @ref: The iterator reference.
 *   &returns: The reference. 'NULL' if at the end of the iterator.
 */

typedef void *(*iter_f)(void *ref);

/**
 * Iterator interface structure.
 *   @next: The next function.
 *   @delete: The internal deletion function.
 */

struct iter_i {
	iter_f next;
	delete_f delete;
};

/**
 * Iterator storage structure.
 *   @ref: The internal reference.
 *   @iface: The iterator interface.
 */

struct iter_t {
	void *ref;
	const struct iter_i *iface;
};


/**
 * Create an iterator.
 *   @ref: The enumerator reference.
 *   &returns: The iterator.
 */

typedef struct iter_t (*enum_f)(void *ref);

/**
 * Enumerator interface structure.
 *   @iter: The iterator function.
 *   @delete: The internal deletion function.
 */

struct enum_i {
	enum_f iter;
	delete_f delete;
};

/**
 * Enumerator storage structure.
 *   @ref: The internal reference.
 *   @iface: The enumerator interface.
 */

struct enum_t {
	void *ref;
	const struct enum_i *iface;
};


/**
 * Filter an input element.
 *   @ref: The filter reference.
 *   @in: The input.
 *   &returns: The output reference or null.
 */

typedef void *(*filter_f)(void *ref, void *in);

/**
 * Filter an input element.
 *   @in: The input.
 *   @arg: The argument.
 *   &returns: The output reference or null.
 */

typedef void *(*filter_apply_f)(void *in, void *arg);

/**
 * Filter interface structure.
 *   @apply: The filter application function.
 *   @delete: The internal deletion function.
 */

struct filter_i {
	filter_f apply;
	delete_f delete;
};

/**
 * Enumerator storage structure.
 *   @ref: The internal reference.
 *   @iface: The filter interface.
 */

struct filter_t {
	void *ref;
	const struct filter_i *iface;
};


/*
 * common implementation variables
 */

extern struct iter_t iter_blank;

/*
 * blank function declarations
 */

void delete_noop(void *ref);


/**
 * AVL tree definitions.
 *   @AVLTREE_MAX_HEIGHT: The maximum height of an AVL tree.
 *   @AVLTREE_NODE_INIT: An inline structure to initialize a node.
 */

#define AVLTREE_MAX_HEIGHT	48
#define AVLTREE_NODE_INIT	(struct avltree_node_t){ 0, NULL, { NULL, NULL } }


/*
 * structure prototypes
 */

struct avltree_t;
struct avltree_node_t;
struct avltree_ref_t;
struct avltree_iter_t;



/**
 * AVL tree storage.
 *   @root: The root node.
 *   @count: The number of nodes.
 *   @compare: The comparison function.
 *   @delete: The value deletion function.
 */

struct avltree_t {
	struct avltree_node_t *root;

	unsigned int count;

	compare_f compare;
	delete_f delete;
};

/**
 * AVL tree node storage.
 *   @balance: The current balance of the node, between '-2' to '2'.
 *   @parent, child: The parent and child nodes.
 */

struct avltree_node_t {
	int8_t balance;
	struct avltree_node_t *parent, *child[2];
};

/**
 * AVL tree reference storage.
 *   @key: The key reference.
 *   @ref: The value reference.
 *   @node: The node.
 */

struct avltree_ref_t {
	const void *key;
	void *ref;

	struct avltree_node_t node;
};

/**
 * AVL tree iterator storage.
 *   @i: The current level.
 *   @stack: The stack of nodes.
 *   @state: The stack of states per level.
 */

struct avltree_iter_t {
	short i;
	struct avltree_node_t *stack[AVLTREE_MAX_HEIGHT];
	uint8_t state[AVLTREE_MAX_HEIGHT];
};

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
