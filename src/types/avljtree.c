#include "../common.h"
#include "avljtree.h"
#include "../debug/exception.h"
#include "../mem/manage.h"


/**
 * Deletion callback information.
 *   @delete: The user deletion callback.
 *   @arg: The deletion argument.
 */

struct delinfo_t {
	avljtree_delete_node_f delete;
	void *arg;
};


/*
 * local function declarations
 */

static void node_delete(struct avltree_node_t *node, void *arg);

static void ref_del(struct avljtree_node_t *node, void *arg);

static int compare_nodekey(const void *key, const struct avltree_node_t *node, void *arg);
static int compare_nodenode(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg);

/*
 * local variables
 */

static struct iter_i keyiter_iface = { (iter_next_f)avljtree_keyiter_next, NULL, mem_free };
static struct iter_i keyiter_keys_iface = { (iter_next_f)avljtree_keyiter_next_key, NULL, mem_free };
static struct iter_i keyiter_refs_iface = { (iter_next_f)avljtree_keyiter_next_ref, NULL, mem_free };

static struct iter_i idxiter_iface = { (iter_next_f)avljtree_idxiter_next, NULL, mem_free };
static struct iter_i idxiter_keys_iface = { (iter_next_f)avljtree_idxiter_next_key, NULL, mem_free };
static struct iter_i idxiter_refs_iface = { (iter_next_f)avljtree_idxiter_next_ref, NULL, mem_free };


/**
 * Initialize the tree root.
 *   @root: The root.
 */

_export
void avljtree_root_init(struct avljtree_root_t *root)
{
	root->bykey = NULL;
	root->byidx = NULL;
}


/**
 * Retrieve the first node sorted by index.
 *   @root: The root.
 *   &returns: The node or null.
 */

_export
struct avljtree_node_t *avljtree_root_idxfirst(struct avljtree_root_t *root)
{
	struct avlitree_node_t *node;

	if(root->byidx == NULL)
		return NULL;

	node = avlitree_node_first(&root->byidx->index);
	if(node == NULL)
		return NULL;

	return avljtree_cast_byidx(node);
}

/**
 * Retrieve the last node sorted by index.
 *   @root: The root.
 *   &returns: The node or null.
 */

_export
struct avljtree_node_t *avljtree_root_idxlast(struct avljtree_root_t *root)
{
	struct avlitree_node_t *node;

	if(root->byidx == NULL)
		return NULL;

	node = avlitree_node_last(&root->byidx->index);
	if(node == NULL)
		return NULL;

	return avljtree_cast_byidx(node);
}


/**
 * Retrieve the node at the given index.
 *   @root: The root.
 *   @index: The index.
 */

_export
struct avljtree_node_t *avljtree_root_get(struct avljtree_root_t *root, unsigned int index)
{
	struct avlitree_node_t *node;

	node = avlitree_node_get(&root->byidx->index, index);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, index);
}


/**
 * Prepend a node to the tree root.
 *   @root: The root.
 *   @node: The node.
 *   @compare: The node-node comparison function.
 *   @arg: Argument passed to the comparison.
 */

_export
void avljtree_root_prepend(struct avljtree_root_t *root, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg)
{
	avljtree_node_prepend(&root->bykey, &root->byidx, node, compare, arg);
}

/**
 * Append a node to the tree root.
 *   @root: The root.
 *   @node: The node.
 *   @compare: The node-node comparison function.
 *   @arg: Argument passed to the comparison.
 */

_export
void avljtree_root_append(struct avljtree_root_t *root, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg)
{
	avljtree_node_append(&root->bykey, &root->byidx, node, compare, arg);
}

/**
 * Insert a node to the tree root at a given index.
 *   @root: The root.
 *   @index: The index.
 *   @node: The node.
 *   @compare: The node-node comparison function.
 *   @arg: Argument passed to the comparison.
 */

_export
void avljtree_root_insert(struct avljtree_root_t *root, unsigned int index, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg)
{
	avljtree_node_insert(&root->bykey, &root->byidx, index, node, compare, arg);
}


/**
 * Remove a node from an AVL joint tree, locating the element by key.
 *   @root: The root.
 *   @key: The sought key.
 *   @compare: The node-key comparison function.
 *   @arg: An argument passed to the comparison function.
 *   &returns: The node if found, 'NULL' if not found.
 */

_export
struct avljtree_node_t *avljtree_root_remove(struct avljtree_root_t *root, const void *key, avltree_compare_nodekey_f compare, void *arg)
{
	return avljtree_node_remove(&root->bykey, &root->byidx, key, compare, arg);
}


/**
 * Initialize an AVL joint tree key iterator across all nodes from the root.
 *   @iter: The tree iterator to initialize.
 *   @root: The keyed root node.
 */

_export
void avljtree_root_keyiter_init(struct avljtree_keyiter_t *iter, struct avljtree_root_t *root)
{
	avltree_node_iter_init(&iter->keyiter, root->bykey ? &root->bykey->key : NULL);
}

/**
 * Retrieve the next node from an AVL joint tree key iterator.
 *   @iter: The iterator.
 *   &returns: The next node, 'NULL' if all nodes are exhausted.
 */

_export
struct avljtree_node_t *avljtree_root_keyiter_next(struct avljtree_keyiter_t *iter)
{
	struct avltree_node_t *node;

	node = avltree_node_iter_next(&iter->keyiter);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, key);
}

/**
 * Retrieve the next node from an AVL joint tree key iterator, depth first.
 *   @iter: The iterator.
 *   &returns: The next node, 'NULL' if all nodes are exhausted.
 */

_export
struct avljtree_node_t *avljtree_root_keyiter_next_depth(struct avljtree_keyiter_t *iter)
{
	struct avltree_node_t *node;

	node = avltree_node_iter_next_depth(&iter->keyiter);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, key);
}


/**
 * Initialize an AVL joint tree index iterator across all nodes from the root.
 *   @iter: The tree iterator to initialize.
 *   @root: The indexed root node.
 */

_export
void avljtree_root_idxiter_init(struct avljtree_idxiter_t *iter, struct avljtree_root_t *root)
{
	iter->idxiter = avlitree_node_iter_new(root->byidx ? &root->byidx->index : NULL);
}

/**
 * Retrieve the next node from an AVL joint tree index iterator.
 *   @iter: The iterator.
 *   &returns: The next node, 'NULL' if all nodes are exhausted.
 */

_export
struct avljtree_node_t *avljtree_root_idxiter_next(struct avljtree_idxiter_t *iter)
{
	struct avlitree_node_t *node;

	node = avlitree_node_iter_next(&iter->idxiter);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, index);
}


/**
 * Obtain the first element from the root based off key-order.
 *   @root: The keyed root node.
 *   &returns: The first node from the root, or 'NULL' if no elements exist.
 */

_export
struct avljtree_node_t *avljtree_node_keyfirst(struct avljtree_node_t *root)
{
	struct avltree_node_t *node;

	node = avltree_node_first(&root->key);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, key);
}

/**
 * Obtain the last element from the root baseed off key-order.
 *   @root: The keyed root node.
 *   &returns: The last node from the root, or 'NULL' if no elements exist.
 */

_export
struct avljtree_node_t *avljtree_node_keylast(struct avljtree_node_t *root)
{
	struct avltree_node_t *node;

	node = avltree_node_last(&root->key);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, key);
}

/**
 * Obtain the first element from the root based off index-order.
 *   @root: The indexed root node.
 *   &returns: The first node from the root, or 'NULL' if no elements exist.
 */

_export
struct avljtree_node_t *avljtree_node_idxfirst(struct avljtree_node_t *root)
{
	struct avlitree_node_t *node;

	if(root == NULL)
		return NULL;

	node = avlitree_node_first(&root->index);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, index);
}

/**
 * Obtain the last element from the root baseed off index-order.
 *   @root: The indexed root node.
 *   &returns: The last node from the root, or 'NULL' if no elements exist.
 */

_export
struct avljtree_node_t *avljtree_node_idxlast(struct avljtree_node_t *root)
{
	struct avlitree_node_t *node;

	if(root == NULL)
		return NULL;

	node = avlitree_node_last(&root->index);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, index);
}

/**
 * Look up an AVL joint tree node from the root using a key.
 *   @root: The keyed root node.
 *   @key: The sought key.
 *   @compare: The node-key comparison function.
 *   @arg: An argument passed to teh comparison function.
 *   &returns: The node if found, 'NULL' if not found.
 */

_export
struct avljtree_node_t *avljtree_node_lookup(struct avljtree_node_t *root, const void *key, avltree_compare_nodekey_f compare, void *arg)
{
	struct avltree_node_t *node;

	node = avltree_node_lookup(&root->key, key, compare, arg);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, key);
}

/**
 * Look up an AVL joint tree node from the root using an index.
 *   @root: The indexed root node.
 *   @index: The index.
 *   &returns: The node if found, 'NULL' if not requested index was too large.
 */

_export
struct avljtree_node_t *avljtree_node_get(struct avljtree_node_t *root, unsigned int index)
{
	struct avlitree_node_t *node;

	node = avlitree_node_get(&root->index, index);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, index);
}

/**
 * Retrieve the index of a given node.
 *   @node: The node.
 *   &returns: The index of the node.
 */

_export
unsigned int avljtree_node_index(struct avljtree_node_t *node)
{
	return avlitree_node_index(&node->index);
}


/**
 * Prepend an AVL joint tree node to the keyed and indexed roots.
 *   @keyroot: A pointer to the keyed root node.
 *   @idxroot: A pointer to the indexed root node.
 *   @node: The node to insert.
 *   @compare: The node-node comparison function.
 *   @arg: An argument passed to the comparison function.
 */

_export
void avljtree_node_prepend(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg)
{
	avljtree_node_insert(keyroot, idxroot, 0, node, compare, arg);
}

/**
 * Prepend an AVL joint tree node to the keyed and indexed roots.
 *   @keyroot: A pointer to the keyed root node.
 *   @idxroot: A pointer to the indexed root node.
 *   @node: The node to insert.
 *   @compare: The node-node comparison function.
 *   @arg: An argument passed to the comparison function.
 */

_export
void avljtree_node_append(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg)
{
	avljtree_node_insert(keyroot, idxroot, UINT_MAX, node, compare, arg);
}

/**
 * Insert an AVL joint tree node to the keyed and indexed roots.
 *   @keyroot: A pointer to the keyed root node.
 *   @idxroot: A pointer to the indexed root node.
 *   @index: The destination index.
 *   @node: The node to insert.
 *   @compare: The node-node comparison function.
 *   @arg: An argument passed to the comparison function.
 */

_export
void avljtree_node_insert(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, unsigned int index, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg)
{
	struct avltree_node_t *keynode = *keyroot ? &(*keyroot)->key : NULL;
	struct avlitree_node_t *idxnode = *idxroot ? &(*idxroot)->index : NULL;

	*node = AVLJTREE_NODE_INIT;

	avltree_node_insert(&keynode, &node->key, compare, arg);
	*keyroot = (void *)keynode - offsetof(struct avljtree_node_t, key);

	avlitree_node_insert(&idxnode, index, &node->index);
	*idxroot = (void *)idxnode - offsetof(struct avljtree_node_t, index);
}

/**
 * Replace the node at the given index.
 *   @keyroot: A pointer to the keyed root node.
 *   @idxroot: A pointer to the indexed root node.
 *   @index: The sought index.
 *   @node: The node to add.
 *   @compare: The node-node comparison function.
 *   @arg: An argument passed to the comparison function.
 *   &returns: The node if found, 'NULL' if not found.
 */

_export
struct avljtree_node_t *avljtree_node_set(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, unsigned int index, struct avljtree_node_t *node, avltree_compare_nodenode_f compare, void *arg)
{
	struct avltree_node_t *keynode = *keyroot ? &(*keyroot)->key : NULL;
	struct avlitree_node_t *idxnode = *idxroot ? &(*idxroot)->index : NULL;
	struct avltree_node_t *keyprev;
	struct avlitree_node_t *idxprev;
	struct avljtree_node_t *prev;

	*node = AVLJTREE_NODE_INIT;

	idxprev = avlitree_node_set(&idxnode, index, &node->index);
	*idxroot = (void *)idxnode - offsetof(struct avljtree_node_t, index);

	if(idxprev == NULL)
		return NULL;

	prev = (void *)idxprev - offsetof(struct avljtree_node_t, index);
	keyprev = avltree_node_remove(&keynode, &prev->key, (avltree_compare_nodekey_f)compare, arg);

	if(&prev->key != keyprev)
		_fatal("Corrupt AVL joint tree.");

	avltree_node_insert(&keynode, &node->key, compare, arg);
	*keyroot = (void *)keynode - offsetof(struct avljtree_node_t, key);

	return prev;
}

/**
 * Replace the node with a given key. The new node will have the old node's
 * index.
 *   @keyroot: A pointer to the keyed root node.
 *   @idxroot: A pointer to the indexed root node.
 *   @node: The node to add.
 *   @key: The sought key.
 *   @compare_k: The node-key comparison function.
 *   @arg_k: An argument passed to the node-key comparison function.
 *   @compare_n: The node-node comparison function.
 *   @arg_n: An argument passed to the node-node comparison function.
 *   &returns: The node if found, 'NULL' if not found.
 */

_export
struct avljtree_node_t *avljtree_node_replace(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, struct avljtree_node_t *node, const void *key, avltree_compare_nodekey_f compare_k, void *arg_k, avltree_compare_nodenode_f compare_n, void *arg_n)
{
	struct avltree_node_t *keynode = *keyroot ? &(*keyroot)->key : NULL;
	struct avlitree_node_t *idxnode = *idxroot ? &(*idxroot)->index : NULL;
	struct avltree_node_t *keyprev;
	struct avlitree_node_t *idxprev;
	struct avljtree_node_t *prev;

	*node = AVLJTREE_NODE_INIT;

	if(compare_k(key, &node->key, arg_k) && avltree_node_lookup(keynode, node, (avltree_compare_nodekey_f)compare_n, arg_k))
		return NULL;

	keyprev = avltree_node_remove(&keynode, key, compare_k, arg_k);
	if(keyprev == NULL)
		return NULL;

	avltree_node_insert(&keynode, &node->key, compare_n, arg_n);
	*keyroot = (void *)keynode - offsetof(struct avljtree_node_t, key);

	prev = (void *)keyprev - offsetof(struct avljtree_node_t, key);
	idxprev = avlitree_node_set(&idxnode, avlitree_node_index(&prev->index), &node->index);

	if(&prev->index != idxprev)
		_fatal("Corrupt AVL joint tree.");

	return prev;
}

/**
 * Remove a node from an AVL joint tree, locating the element by index.
 *   @keyroot: A pointer to the keyed root node.
 *   @idxroot: A pointer to the indexed root node.
 *   @index: The sought index.
 *   @compare: The node-node comparison function.
 *   @arg: An argument passed to the comparison function.
 *   &returns: The node if found, 'NULL' if not found.
 */

_export
struct avljtree_node_t *avljtree_node_slice(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, unsigned int index, avltree_compare_nodenode_f compare, void *arg)
{
	struct avltree_node_t *keyret, *keynode = &(*keyroot)->key;
	struct avlitree_node_t *idxret, *idxnode = &(*idxroot)->index;
	struct avljtree_node_t *node;

	idxret = avlitree_node_slice(&idxnode, index);
	*idxroot = (void *)idxnode - offsetof(struct avljtree_node_t, index);

	if(idxret == NULL)
		return NULL;

	node = (void *)idxret - offsetof(struct avljtree_node_t, index);

	keyret = avltree_node_remove(&keynode, &node->key, (avltree_compare_nodekey_f)compare, arg);
	*keyroot = (void *)keynode - offsetof(struct avljtree_node_t, key);

	if(&node->key != keyret)
		_fatal("Corrupt AVL joint tree.");

	return node;
}

/**
 * Remove a node from an AVL joint tree, locating the element by key.
 *   @keyroot: A pointer to the keyed root node.
 *   @idxroot: A pointer to the indexed root node.
 *   @key: The sought key.
 *   @compare: The node-key comparison function.
 *   @arg: An argument passed to the comparison function.
 *   &returns: The node if found, 'NULL' if not found.
 */

_export
struct avljtree_node_t *avljtree_node_remove(struct avljtree_node_t **keyroot, struct avljtree_node_t **idxroot, const void *key, avltree_compare_nodekey_f compare, void *arg)
{
	struct avltree_node_t *keyret, *keynode = &(*keyroot)->key;
	struct avlitree_node_t *idxret, *idxnode = &(*idxroot)->index;
	struct avljtree_node_t *node;

	keyret = avltree_node_remove(&keynode, key, compare, arg);
	*keyroot = (void *)keynode - offsetof(struct avljtree_node_t, key);

	if(keyret == NULL)
		return NULL;

	node = (void *)keyret - offsetof(struct avljtree_node_t, key);

	idxret = avlitree_node_slice(&idxnode, avlitree_node_index(&node->index));
	*idxroot = (void *)idxnode - offsetof(struct avljtree_node_t, index);

	if(&node->index != idxret)
		_fatal("Corrupt AVL joint tree.");

	return node;
}

/**
 * Clear all nodes under a root through a deletion callback.
 *   @root: The root node.
 *   @delete: The deletion callback.
 */

_export
void avljtree_node_clear(struct avljtree_node_t *root, avljtree_delete_node_f delete, void *arg)
{
	struct delinfo_t info = { delete, arg };

	avltree_node_clear(&root->key, node_delete, &info);
}

/**
 * Deletion callback to hook into the base AVL tree clear.
 *   @node: The AVL tree node.
 *   @arg: The deletion information.
 */

static void node_delete(struct avltree_node_t *node, void *arg)
{
	struct delinfo_t *info = arg;

	info->delete((void *)node - offsetof(struct avljtree_node_t, key), info->arg);
}


/**
 * Create an AVL joint tree key iterator across all nodes from the root.
 *   @root: The keyed root node.
 *   &returns: The iterator.
 */

_export
struct avljtree_keyiter_t avljtree_node_keyiter_new(struct avljtree_node_t *root)
{
	struct avljtree_keyiter_t iter;

	avltree_node_iter_init(&iter.keyiter, root ? &root->key : NULL);

	return iter;
}

/**
 * Retrieve the next node from an AVL joint tree key iterator.
 *   @iter: The iterator.
 *   &returns: The next node, 'NULL' if all nodes are exhausted.
 */

_export
struct avljtree_node_t *avljtree_node_keyiter_next(struct avljtree_keyiter_t *iter)
{
	struct avltree_node_t *node;

	node = avltree_node_iter_next(&iter->keyiter);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, key);
}


/**
 * Initialize an AVL joint tree index iterator across all nodes from the root.
 *   @iter: The tree iterator to initialize.
 *   @root: The indexed root node.
 */

_export
void avljtree_node_idxiter_init(struct avljtree_idxiter_t *iter, struct avljtree_node_t *root)
{
	iter->idxiter = avlitree_node_iter_new(root ? &root->index : NULL);
}

/**
 * Retrieve the next node from an AVL joint tree index iterator.
 *   @iter: The iterator.
 *   &returns: The next node, 'NULL' if all nodes are exhausted.
 */

_export
struct avljtree_node_t *avljtree_node_idxiter_next(struct avljtree_idxiter_t *iter)
{
	struct avlitree_node_t *node;

	node = avlitree_node_iter_next(&iter->idxiter);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_node_t, index);
}


/**
 * Initialize the AVL join tree.
 *   @tree: The uninitialized AVL join tree.
 *   @compare: The comparison function used to sort reference.
 *   @delete: Optional. The callback to delete references. Set to 'NULL' if
 *     unused.
 *   &prop: noerror
 */

_export
void avljtree_init(struct avljtree_t *tree, compare_f compare, delete_f delete)
{
	*tree = (struct avljtree_t){ NULL, NULL, 0, compare, delete };
}

/**
 * Allocates and initializes a new AVL join tree.
 *   @compare: The comparison function used to sort reference.
 *   @delete: Optional. The callback to delete references. Set to 'NULL' if
 *     unused.
 *   &returns: The AVL join tree.
 */

_export
struct avljtree_t *avljtree_new(compare_f compare, delete_f delete)
{
	struct avljtree_t *tree;

	tree = mem_alloc(sizeof(struct avljtree_t));
	avljtree_init(tree, compare, delete);

	return tree;
}

/**
 * Cleans up all data associated with AVL joint tree and all its references.
 *   @tree: The AVL joint tree.
 */

_export
void avljtree_destroy(struct avljtree_t *tree)
{
	avljtree_clear(tree);
}

/**
 * Deletes the AVL joint tree an all its references.
 *   @tree: The AVL joint tree.
 */

_export
void avljtree_delete(struct avljtree_t *tree)
{
	avljtree_destroy(tree);

	mem_free(tree);
}


/**
 * Obtain the first element in the tree in key order.
 *   @tree: The AVL joint tree.
 *   &returns: The reference from the first element, or 'NULL' if the tree is
 *     empty.
 */

_export
void *avljtree_first(const struct avljtree_t *tree)
{
	struct avljtree_node_t *node;

	node = avljtree_node_keyfirst(tree->keyroot);
	if(node == NULL)
		return NULL;

	return ((struct avljtree_ref_t *)((void *)node - offsetof(struct avljtree_ref_t, node)))->ref;
}

/**
 * Obtain the last element in the tree in key order.
 *   @tree: The AVL joint tree.
 *   &returns: The reference from the last element, or 'NULL' if the tree is
 *     empty.
 */

_export
void *avljtree_last(const struct avljtree_t *tree)
{
	struct avljtree_node_t *node;

	node = avljtree_node_keyfirst(tree->keyroot);
	if(node == NULL)
		return NULL;

	return ((struct avljtree_ref_t *)((void *)node - offsetof(struct avljtree_ref_t, node)))->ref;
}

/**
 * Lookup an AVL joint tree reference based on key.
 *   @tree: The AVL joint tree.
 *   @key: The sought key.
 *   &returns: The reference if found, 'NULL' if not found.
 */

_export
void *avljtree_lookup(const struct avljtree_t *tree, const void *key)
{
	struct avljtree_node_t *node;

	node = avljtree_node_lookup(tree->keyroot, key, compare_nodekey, tree->compare);
	if(node == NULL)
		return NULL;

	return ((struct avljtree_ref_t *)((void *)node - offsetof(struct avljtree_ref_t, node)))->ref;
}

/**
 * Retrieves the reference from the given index.
 *   @tree: The AVL joint tree.
 *   @index: The index.
 *   &returns: The reference if found, 'NULL' if the given index does not
 *     exist.
 */

_export
void *avljtree_get(const struct avljtree_t *tree, unsigned int index)
{
	struct avljtree_node_t *node;

	node = avljtree_node_get(tree->idxroot, index);
	if(node == NULL)
		return NULL;

	return ((struct avljtree_ref_t *)((void *)node - offsetof(struct avljtree_ref_t, node)))->ref;
}

/**
 * Determine the index from a given key.
 *   @tree: The AVL joint tree.
 *   @key: The sought key.
 *   &returns: If found, the key's associated index, otherwise
 *     '(unsigned int)-1'.
 */

_export
unsigned int avljtree_index(const struct avljtree_t *tree, const void *key)
{
	struct avljtree_node_t *node;

	node = avljtree_node_lookup(tree->keyroot, key, compare_nodekey, tree->compare);
	if(node == NULL)
		return (unsigned int)-1;

	return avljtree_node_index(node);
}


/**
 * Insert a reference into the AVL joint tree.
 *   @tree: The AVL joint tree.
 *   @index: The index.
 *   @key: The key reference.
 *   @ref: The value reference.
 */

_export
void avljtree_insert(struct avljtree_t *tree, unsigned int index, const void *key, void *ref)
{
	struct avljtree_ref_t *value;

	value = mem_alloc(sizeof(struct avljtree_ref_t));
	value->ref = ref;
	value->key = key;
	
	avljtree_node_insert(&tree->keyroot, &tree->idxroot, index, &value->node, compare_nodenode, tree->compare);
	tree->count++;
}

/**
 * Replace the reference at the given index.
 *   @tree: The AVL joint tree.
 *   @index: The index.
 *   @key: The key reference.
 *   @ref: The value reference.
 *   &returns: The previous reference if found, 'NULL' otherwise.
 */

_export
void *avljtree_set(struct avljtree_t *tree, unsigned int index, const void *key, void *ref)
{
	struct avljtree_ref_t *value;
	struct avljtree_node_t *node;

	value = mem_alloc(sizeof(struct avljtree_ref_t));
	value->ref = ref;
	value->key = key;
	
	node = avljtree_node_set(&tree->keyroot, &tree->idxroot, index, &value->node, compare_nodenode, tree->compare);
	if(node != NULL) {
		value = (void *)node - offsetof(struct avljtree_ref_t, node);
		ref = value->ref;
	}
	else
		ref = NULL;

	mem_free(value);

	return ref;
}

/**
 * Replace the reference that matches the given key. The new reference will
 * retain the previous index.
 *   @tree: The AVL joint tree.
 *   @index: The index.
 *   @key: The key reference.
 *   @ref: The value reference.
 *   &returns: The previous reference if found, 'NULL' otherwise.
 */

_export
void *avljtree_replace(struct avljtree_t *tree, const void *orig, const void *key, void *ref)
{
	struct avljtree_ref_t *value;
	struct avljtree_node_t *node;

	value = mem_alloc(sizeof(struct avljtree_ref_t));
	value->ref = ref;
	value->key = key;
	
	node = avljtree_node_replace(&tree->keyroot, &tree->idxroot, &value->node, orig, compare_nodekey, tree->compare, compare_nodenode, tree->compare);
	if(node != NULL) {
		value = (void *)node - offsetof(struct avljtree_ref_t, node);
		ref = value->ref;
	}
	else
		ref = NULL;

	mem_free(value);

	return ref;
}

/**
 * Replace a matching key, deleting the old reference.
 *   @tree: The AVL joint tree.
 *   @index: The index.
 *   @key: The key reference.
 *   @ref: The value reference.
 */

_export
void avljtree_displace(struct avljtree_t *tree, const void *orig, const void *key, void *ref)
{
	ref = avljtree_replace(tree, orig, key, ref);
	if(ref == NULL)
		throw("Key not found.");

	if(tree->delete != NULL)
		tree->delete(ref);
}

/**
 * Remove a reference at the given index.
 *   @tree: The AVL joint tree.
 *   @index: The index.
 *   &returns: The value reference if found, 'NULL' otherwise.
 */

_export
void *avljtree_slice(struct avljtree_t *tree, unsigned int index)
{
	struct avljtree_node_t *node;
	struct avljtree_ref_t *ref;
	void *value;

	node = avljtree_node_slice(&tree->keyroot, &tree->idxroot, index, compare_nodenode, tree->compare);
	if(node == NULL)
		return NULL;

	ref = (void *)node - offsetof(struct avltree_ref_t, node);
	value = ref->ref;

	mem_free(ref);
	tree->count--;

	return value;
}

/**
 * Remove and delete a reference from the AVL joint tree using an index.
 *   @tree: The AVL joint tree.
 *   @index: The index.
 */

_export
void avljtree_erase(struct avljtree_t *tree, unsigned int index)
{
	void *ref;

	ref = avljtree_slice(tree, index);
	if(ref == NULL)
		throw("Index too large.");

	if(tree->delete != NULL)
		tree->delete(ref);
}

/**
 * Remove a reference from an AVL joint tree using a key.
 *   @tree: The AVL joint tree.
 *   @key: The key reference.
 *   &returns: The value reference if found, 'NULL' otherwise.
 */

_export
void *avljtree_remove(struct avljtree_t *tree, const void *key)
{
	struct avljtree_node_t *node;
	struct avljtree_ref_t *ref;
	void *value;

	node = avljtree_node_remove(&tree->keyroot, &tree->idxroot, key, compare_nodekey, tree->compare);
	if(node == NULL)
		return NULL;

	ref = (void *)node - offsetof(struct avltree_ref_t, node);
	value = ref->ref;

	mem_free(ref);
	tree->count--;

	return value;
}

/**
 * Removes and delete a reference from the AVL joint tree.
 *   @tree: The AVL joint tree.
 *   @key: The key reference.
 */

_export
void avljtree_purge(struct avljtree_t *tree, const void *key)
{
	void *ref;

	ref = avljtree_remove(tree, key);
	if(ref == NULL)
		throw("Key not found.");

	if(tree->delete != NULL)
		tree->delete(ref);
}

/**
 * Delete every reference from a tree, leaving it empty.
 *   @tree: The AVL joint tree.
 *   &prop: noerror
 */

_export
void avljtree_clear(struct avljtree_t *tree)
{
	avljtree_node_clear(tree->keyroot, ref_del, tree->delete);

	tree->keyroot = NULL;
	tree->idxroot = NULL;
	tree->count = 0;
}


/**
 * Append a reference to the end of the AVL joint tree. All indices of
 * existing elements will remain the same.
 *   @tree: The AVL joint tree.
 *   @key: The key reference.
 *   @ref: The value reference.
 */

_export
void avljtree_append(struct avljtree_t *tree, const void *key, void *ref)
{
	avljtree_insert(tree, tree->count, key, ref);
}

/**
 * Add a reference to the beginning of the AVL joint tree. All indices of
 * existing elements will be incremented by one.
 *   @tree: The AVL joint tree.
 *   @key: The key reference.
 *   @ref: The value reference.
 */

_export
void avljtree_prepend(struct avljtree_t *tree, const void *key, void *ref)
{
	avljtree_insert(tree, 0, key, ref);
}


/**
 * Create a key iterator on the AVL joint tree.
 *   @tree: The AVL joint tree.
 *   &returns: The iterator.
 */

_export
struct avljtree_keyiter_t avljtree_keyiter_new(const struct avljtree_t *tree)
{
	return avljtree_node_keyiter_new(tree->keyroot);
}

/**
 * Retrieve the next reference from an AVL joint tree key iterator.
 *   @iter: The iterator.
 *   &returns: The next reference, 'NULL' if all references are exhausted.
 */

_export
void *avljtree_keyiter_next(struct avljtree_keyiter_t *iter)
{
	struct avljtree_ref_t *ref;

	ref = avljtree_keyiter_next_ref(iter);
	return ref ? ref->ref : NULL;
}

/**
 * Retrieve the next key reference from an AVL joint tree key iterator.
 *   @iter: The iterator.
 *   &returns: The next key reference, 'NULL' if all references are exhausted.
 */

_export
void *avljtree_keyiter_next_key(struct avljtree_keyiter_t *iter)
{
	struct avljtree_ref_t *ref;

	ref = avljtree_keyiter_next_ref(iter);
	return ref ? (void *)ref->key : NULL;
}

/**
 * Retrieve the next reference structure from an AVL joint tree key iterator.
 *   @iter: The iterator.
 *   &returns: The next key reference, 'NULL' if all references are exhausted.
 */

_export
struct avljtree_ref_t *avljtree_keyiter_next_ref(struct avljtree_keyiter_t *iter)
{
	struct avljtree_node_t *node;

	node = avljtree_node_keyiter_next(iter);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_ref_t, node);
}


/**
 * Create a new key iterator over the references in an AVL joint tree.
 *   @tree: The AVL joint tree.
 *   &returns: The iterator.
 */

_export
struct iter_t avljtree_keyiter_vals_new(struct avljtree_t *tree)
{
	struct iter_t iter;

	iter.ref = mem_alloc(sizeof(struct avljtree_keyiter_t));
	iter.iface = &keyiter_iface;
	*(struct avljtree_keyiter_t *)iter.ref = avljtree_keyiter_new(tree);

	return iter;
}

/**
 * Create a new key iterator over the keys in an AVL joint tree.
 *   @tree: The AVL joint tree.
 *   &returns: The iterator.
 */

_export
struct iter_t avljtree_keyiter_keys_new(struct avljtree_t *tree)
{
	struct iter_t iter;

	iter.ref = mem_alloc(sizeof(struct avljtree_keyiter_t));
	iter.iface = &keyiter_keys_iface;
	*(struct avljtree_keyiter_t *)iter.ref = avljtree_keyiter_new(tree);

	return iter;
}

/**
 * Create a new key iterator over the reference strutures in an AVL joint
 * tree.
 *   @tree: The AVL joint tree.
 *   &returns: The iterator.
 */

_export
struct iter_t avljtree_keyiter_refs_new(struct avljtree_t *tree)
{
	struct iter_t iter;

	iter.ref = mem_alloc(sizeof(struct avljtree_keyiter_t));
	iter.iface = &keyiter_refs_iface;
	*(struct avljtree_keyiter_t *)iter.ref = avljtree_keyiter_new(tree);

	return iter;
}


/**
 * Initialize an index iterator on the AVL joint tree.
 *   @iter: The uninitailized iterator structure.
 *   @tree: The AVL joint tree.
 */

_export
void avljtree_idxiter_init(struct avljtree_idxiter_t *iter, const struct avljtree_t *tree)
{
	avljtree_node_idxiter_init(iter, tree->idxroot);
}

/**
 * Retrieve the next reference from an AVL joint tree index iterator.
 *   @iter: The iterator.
 *   &returns: The next reference, 'NULL' if all references are exhausted.
 */

_export
void *avljtree_idxiter_next(struct avljtree_idxiter_t *iter)
{
	struct avljtree_ref_t *ref;

	ref = avljtree_idxiter_next_ref(iter);
	return ref ? ref->ref : NULL;
}

/**
 * Retrieve the next key reference from an AVL joint tree index iterator.
 *   @iter: The iterator.
 *   &returns: The next key reference, 'NULL' if all references are exhausted.
 */

_export
void *avljtree_idxiter_next_key(struct avljtree_idxiter_t *iter)
{
	struct avljtree_ref_t *ref;

	ref = avljtree_idxiter_next_ref(iter);
	return ref ? (void *)ref->key : NULL;
}

/**
 * Retrieve the next reference structure from an AVL joint tree index iterator.
 *   @iter: The iterator.
 *   &returns: The next key reference, 'NULL' if all references are exhausted.
 */

_export
struct avljtree_ref_t *avljtree_idxiter_next_ref(struct avljtree_idxiter_t *iter)
{
	struct avljtree_node_t *node;

	node = avljtree_node_idxiter_next(iter);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avljtree_ref_t, node);
}


/**
 * Create a new index iterator over the references in an AVL joint tree.
 *   @tree: The AVL joint tree.
 *   &returns: The iterator.
 */

_export
struct iter_t avljtree_idxiter_vals_new(struct avljtree_t *tree)
{
	struct iter_t iter;

	iter.ref = mem_alloc(sizeof(struct avljtree_idxiter_t));
	iter.iface = &idxiter_iface;

	avljtree_idxiter_init(iter.ref, tree);

	return iter;
}

/**
 * Create a new index iterator over the keys in an AVL joint tree.
 *   @tree: The AVL joint tree.
 *   &returns: The iterator.
 */

_export
struct iter_t avljtree_idxiter_keys_new(struct avljtree_t *tree)
{
	struct iter_t iter;

	iter.ref = mem_alloc(sizeof(struct avljtree_idxiter_t));
	iter.iface = &idxiter_keys_iface;

	avljtree_idxiter_init(iter.ref, tree);

	return iter;
}

/**
 * Create a new index iterator over the reference strutures in an AVL joint
 * tree.
 *   @tree: The AVL joint tree.
 *   &returns: The iterator.
 */

_export
struct iter_t avljtree_idxiter_refs_new(struct avljtree_t *tree)
{
	struct iter_t iter;

	iter.ref = mem_alloc(sizeof(struct avljtree_idxiter_t));
	iter.iface = &idxiter_refs_iface;

	avljtree_idxiter_init(iter.ref, tree);

	return iter;
}


/**
 * Delete a reference while clearing.
 *   @node: The node.
 *   @arg: The deletion callback.
 */

static void ref_del(struct avljtree_node_t *node, void *arg)
{
	delete_f delete = arg;
	struct avljtree_ref_t *ref = (void *)node - offsetof(struct avljtree_ref_t, node);

	if(delete != NULL)
		delete(ref->ref);

	mem_free(ref);
}


/**
 * Compare a reference node against a key.
 *   @key: The key reference.
 *   @node: The node.
 *   @arg: The reference comparison.
 *   &returns: An integer representing their order.
 */

static int compare_nodekey(const void *key, const struct avltree_node_t *node, void *arg)
{
	compare_f compare = arg;
	struct avljtree_ref_t *ref = (void *)node - offsetof(struct avljtree_ref_t, node) - offsetof(struct avljtree_node_t, key);

	return compare(key, ref->key);
}

/**
 * Compare two reference nodes against one another.
 *   @n1: The first node.
 *   @n2: The second node.
 *   @arg: The reference comparison.
 *   &returns: An integer representing their order.
 */

static int compare_nodenode(const struct avltree_node_t *n1, const struct avltree_node_t *n2, void *arg)
{
	compare_f compare = arg;
	struct avljtree_ref_t *r1 = (void *)n1 - offsetof(struct avljtree_ref_t, node) - offsetof(struct avljtree_node_t, key);
	struct avljtree_ref_t *r2 = (void *)n2 - offsetof(struct avljtree_ref_t, node) - offsetof(struct avljtree_node_t, key);

	return compare(r1->key, r2->key);
}
