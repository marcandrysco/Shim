#include "../common.h"
#include "avlitree.h"
#include "../debug/exception.h"
#include "../mem/manage.h"


/*
 * Node definitions and macros.
 *   @LEFT: The left child index.
 *   @RIGHT: The right child index.
 *   @CMP2NODE: Comparison to child index macro.
 *   @OTHERNODE: Retrieves the opposite child index macro.
 *   @NODEDIR: Child node index to direction macro.
 *   @MYDIR: The direction of the current node from its parent.
 */

#define LEFT	0
#define RIGHT	1
#define CMP2NODE(cmp)	(cmp > 0 ? RIGHT : LEFT)
#define OTHERNODE(node)	(node == RIGHT ? LEFT : RIGHT)
#define NODEDIR(node)	(node == RIGHT ? 1 : -1)
#define MYDIR(node)	(node->parent->child[LEFT] == node ? LEFT : RIGHT)


/*
 * local function declarations
 */

static void ref_del(struct avlitree_node_t *node, void *arg);

static struct avlitree_node_t *rotate_single(struct avlitree_node_t *node, uint8_t dir);
static struct avlitree_node_t *rotate_double(struct avlitree_node_t *node, uint8_t dir);
static void recount(struct avlitree_node_t *node);

/*
 * local variables
 */

static struct iter_i iter_iface = { (iter_next_f)avlitree_iter_next, NULL, mem_free };
static struct iter_i refiter_iface = { (iter_next_f)avlitree_iter_next_ref, NULL, mem_free };


/**
 * Retrieve the first node from the tree.
 *   @root: The root.
 *   &returns: The first node from the root, or 'NULL' if no elements exist.
 */

_export
struct avlitree_node_t *avlitree_node_first(struct avlitree_node_t *root)
{
	if(root == NULL)
		return NULL;

	while(root->child[LEFT] != NULL)
		root = root->child[LEFT];

	return root;
}

/**
 * Retrieve the last node from the tree.
 *   @root: The root.
 *   &returns: The last node from the root, or 'NULL' if no elements exist.
 */

_export
struct avlitree_node_t *avlitree_node_last(struct avlitree_node_t *root)
{
	if(root == NULL)
		return NULL;

	while(root->child[RIGHT] != NULL)
		root = root->child[RIGHT];

	return root;
}

/**
 * Look up an AVL index tree node from the root.
 *   @root: The root node.
 *   @index: The index.
 *   &returns: The node if found, 'NULL' if not requested index was too large.
 */

_export
struct avlitree_node_t *avlitree_node_get(struct avlitree_node_t *root, unsigned int index)
{
	int cmp;
	unsigned int cur, left;
	struct avlitree_node_t *node = root;

	cur = 0;

	while(node != NULL) {
		left = cur;

		if(node->child[LEFT] != NULL)
			left += node->child[LEFT]->count;

		cmp = (int)index - (int)left;
		if(cmp == 0)
			return node;

		if(CMP2NODE(cmp) == RIGHT)
			cur = left + 1;

		node = node->child[CMP2NODE(cmp)];
	}

	if(cur <= index)
		return NULL;

	_fatal("Invalid tree data.");
}

/**
 * Retrieve the index of a given node.
 *   @node: The node.
 *   &returns: The index of the node.
 */

_export
unsigned int avlitree_node_index(struct avlitree_node_t *node)
{
	unsigned int cur;
	struct avlitree_node_t *parent;

	if(node->child[LEFT] != NULL)
		cur = node->child[LEFT]->count;
	else
		cur = 0;

	while(node->parent != NULL) {
		parent = node->parent;

		if(parent->child[RIGHT] == node) {
			cur += 1;

			if(parent->child[LEFT] != NULL)
				cur += parent->child[LEFT]->count;
		}

		node = parent;
	}

	return cur;
}


/**
 * Insert a new node at the given index.
 *   @root: A pointer to the root node.
 *   @index: The destination index.
 *   @ref: A reference to the item.
 */

_export
void avlitree_node_insert(struct avlitree_node_t **root, unsigned int index, struct avlitree_node_t *node)
{
	short i, ii;
	unsigned int cur, left;
	uint8_t dir[AVLITREE_MAX_HEIGHT];
	struct avlitree_node_t *stack[AVLITREE_MAX_HEIGHT];

	*node = AVLITREE_NODE_INIT;

	if(*root == NULL) {
		*root = node;
		node->parent = NULL;

		return;
	}

	cur = 0;
	stack[0] = *root;

	for(i = 0; i < AVLITREE_MAX_HEIGHT; i++) {
		if(stack[i] == NULL)
			break;

		left = cur;

		if(stack[i]->child[LEFT] != NULL)
			left += stack[i]->child[LEFT]->count;

		dir[i] = index > left ? RIGHT : LEFT;
		if(dir[i] == RIGHT)
			cur = left + 1;

		stack[i+1] = stack[i]->child[dir[i]];

	}
	if(i == AVLITREE_MAX_HEIGHT)
		throw("Tree too tall.");

	i--;
	stack[i]->child[dir[i]] = node;
	node->parent = stack[i];

	for(ii = i; ii > -1; ii--)
		stack[ii]->count++;
	
	stack[i]->balance += NODEDIR(dir[i]);

	if(stack[i]->child[OTHERNODE(dir[i])] != NULL)
		return;

	while(i-- > 0) {
		struct avlitree_node_t *node;

		stack[i]->balance += NODEDIR(dir[i]);

		if(stack[i]->balance == 0)
			break;

		if((stack[i]->balance > -2) && (stack[i]->balance < 2))
			continue;

		if(dir[i+1] == CMP2NODE(stack[i]->balance))
			node = rotate_single(stack[i], OTHERNODE(CMP2NODE(stack[i]->balance)));
		else
			node = rotate_double(stack[i], OTHERNODE(CMP2NODE(stack[i]->balance)));

		if(i == 0)
			*root = node;
		else
			stack[i-1]->child[dir[i-1]] = node;
		
		break;
	}
}

/**
 * Remove a node from the AVL index tree.
 *   @root: A pointer to the root node.
 *   @index: The index of the element to remove.
 *   &returns: The removed node, or 'NULL' if the index is too larrge.
 */

_export
struct avlitree_node_t *avlitree_node_slice(struct avlitree_node_t **root, unsigned int index)
{
	short i, ii;
	unsigned int cur, left;
	uint8_t dir[AVLITREE_MAX_HEIGHT];
	struct avlitree_node_t *stack[AVLITREE_MAX_HEIGHT], *node, *retval;

	cur = 0;
	stack[0] = *root;

	for(i = 0; i < AVLITREE_MAX_HEIGHT; i++) {
		if(stack[i] == NULL)
			return NULL;

		left = cur;

		if(stack[i]->child[LEFT] != NULL)
			left += stack[i]->child[LEFT]->count;

		if(index == left)
			break;

		dir[i] = index > left ? RIGHT : LEFT;
		if(dir[i] == RIGHT)
			cur = left + 1;

		stack[i+1] = stack[i]->child[dir[i]];
	}

	for(ii = i - 1; ii != -1; ii--)
		stack[ii]->count--;

	dir[i] = CMP2NODE(stack[i]->balance);

	ii = i;
	node = stack[i]->child[dir[i]];
	if(node != NULL) {
		while(node->child[OTHERNODE(dir[ii])] != NULL) {
			i++;
			stack[i] = node;
			dir[i] = OTHERNODE(dir[ii]);
			node = node->child[dir[i]];
		}

		stack[i]->child[dir[i]] = node->child[dir[ii]];
		i++;

		if(stack[ii]->child[LEFT] != NULL)
			stack[ii]->child[LEFT]->parent = node;

		if(stack[ii]->child[RIGHT] != NULL)
			stack[ii]->child[RIGHT]->parent = node;

		node->child[LEFT] = stack[ii]->child[LEFT];
		node->child[RIGHT] = stack[ii]->child[RIGHT];
		node->balance = stack[ii]->balance;
	}

	if(ii == 0) {
		*root = node;

		if(node != NULL)
			node->parent = NULL;
	}
	else {
		stack[ii-1]->child[dir[ii-1]] = node;

		if(node != NULL)
			node->parent = stack[ii-1];
	}

	retval = stack[ii];
	stack[ii] = node;

	while(i-- > 0) {
		stack[i]->balance -= NODEDIR(dir[i]);

		if((stack[i]->balance > 1) || (stack[i]->balance < -1)) {
			if(stack[i]->balance == -2 * stack[i]->child[CMP2NODE(stack[i]->balance/2)]->balance)
				node = rotate_double(stack[i], OTHERNODE(CMP2NODE(stack[i]->balance)));
			else
				node = rotate_single(stack[i], OTHERNODE(CMP2NODE(stack[i]->balance)));

			if(i == 0)
				*root = node;
			else
				stack[i-1]->child[dir[i-1]] = node;

			stack[i] = node;
		}

		if(stack[i]->balance != 0)
			break;
	}

	return retval;
}

/**
 * Given an index, replace the node. The node will not be added if the index
 * does not exist.
 *   @root: A pointer to the root of the tree.
 *   @index: The index.
 *   @node: The node to add.
 *   &returns: The displaced node if foudn, 'NULL' if the requested index was
 *     too large.
 */

_export
struct avlitree_node_t *avlitree_node_set(struct avlitree_node_t **root, unsigned int index, struct avlitree_node_t *node)
{
	unsigned int cur, left;
	struct avlitree_node_t *sel = *root;

	cur = 0;

	while(sel != NULL) {
		left = cur;

		if(sel->child[LEFT] != NULL)
			left += sel->child[LEFT]->count;

		if(index == left)
			break;

		if(index > left)
			cur = left + 1;

		sel = sel->child[index > left ? RIGHT : LEFT];
	}

	if(sel == NULL) {
		if(cur >= index)
			return NULL;

		_fatal("Invalid tree data.");
	}

	if(sel == *root)
		*root = node;
	else
		sel->parent->child[MYDIR(sel)] = node;

	*node = *sel;

	return sel;
}

/**
 * Clear all nodes under a root through a deletion callback.
 *   @root: The root node.
 *   @delete: The deletion callback.
 */

_export
void avlitree_node_clear(struct avlitree_node_t *root, avlitree_delete_node_f delete, void *arg)
{
	short i;
	struct avlitree_node_t *stack[AVLITREE_MAX_HEIGHT];
	uint8_t stack_s[AVLITREE_MAX_HEIGHT];

	if(root == NULL)
		return;

	stack[0] = root;
	stack_s[0] = 0;

	for(i = 0; i >= 0; ) {
		switch(stack_s[i]) {
		case 0:
			stack_s[i]++;
			if(stack[i]->child[LEFT] != NULL) {
				stack[i+1] = stack[i]->child[LEFT];
				stack_s[i+1] = 0;
				i++;

				break;
			}

		case 1:
			stack_s[i]++;
			if(stack[i]->child[RIGHT] != NULL) {
				stack[i+1] = stack[i]->child[RIGHT];
				stack_s[i+1] = 0;
				i++;

				break;
			}

		case 2:
			delete(stack[i--], arg);
		}
	}
}


/**
 * Initialize an AVL index tree iterator across all nodes from the root.
 *   @iter: The tree iterator to initialize.
 *   @root: The root node.
 */

_export
struct avlitree_iter_t avlitree_node_iter_new(struct avlitree_node_t *root)
{
	struct avlitree_iter_t iter;

	if(root != NULL) {
		iter.i = 0;
		iter.stack[0] = root;
		iter.state[0] = 0;
	}
	else
		iter.i = -2;

	return iter;
}

/**
 * Retrieve the next node from an AVL index tree iterator.
 *   @iter: The iterator.
 *   &returns: The next node, 'NULL' if all nodes are exhausted.
 */

_export
struct avlitree_node_t *avlitree_node_iter_next(struct avlitree_iter_t *iter)
{
	struct avlitree_node_t **stack;
	uint8_t *state;

	if(iter->i < 0)
		return NULL;

	stack = iter->stack;
	state = iter->state;

	for(;;) {
		short i = iter->i;

		switch(state[i]) {
		case 0:
			state[i]++;
			if(stack[i]->child[LEFT] != NULL) {
				stack[i+1] = stack[i]->child[LEFT];
				state[i+1] = 0;
				iter->i++;

				break;
			}

		case 1:
			state[i]++;

			return stack[i];

		case 2:
			state[i]++;
			if(stack[i]->child[RIGHT] != NULL) {
				stack[i+1] = stack[i]->child[RIGHT];
				state[i+1] = 0;
				iter->i++;

				break;
			}

		case 3:
			if(--iter->i < 0)
				return NULL;
		}
	}
}


/**
 * Initialize the AVL index tree.
 *   @tree: The uninitialized AVL index tree.
 *   @delete: Optional. The callback to delete references. Set to 'NULL' if
 *     unused.
 *   &prop: noerror
 */

_export
void avlitree_init(struct avlitree_t *tree, delete_f delete)
{
	tree->root = NULL;
	tree->count = 0;
	tree->delete = delete;
}

/**
 * Allocates and initializes a new AVL index tree.
 *   @delete: Optional. The callback to delete references. Set to 'NULL' if
 *     unused.
 *   &returns: The AVL index tree.
 */

_export
struct avlitree_t *avlitree_new(delete_f delete)
{
	struct avlitree_t *tree;

	tree = mem_alloc(sizeof(struct avlitree_t));
	avlitree_init(tree, delete);

	return tree;
}

/**
 * Cleans up all data associated with AVL index tree and all its references.
 *   @tree: The AVL index tree.
 *   &prop: noerror
 */

_export
void avlitree_destroy(struct avlitree_t *tree)
{
	avlitree_clear(tree);
}

/**
 * Deletes the AVL index tree an all its references.
 *   @tree: The AVL index tree.
 *   &prop: noerror
 */

_export
void avlitree_delete(struct avlitree_t *tree)
{
	avlitree_destroy(tree);
	mem_free(tree);
}


/**
 * Retrieves the values from the given index.
 *   @tree: The AVL index tree.
 *   @index: The index.
 *   &returns: The value if found, otherwise returns 'NULL' if the given index
 *     does not exist.
 */

_export
void *avlitree_get(const struct avlitree_t *tree, unsigned int index)
{
	struct avlitree_node_t *node;

	node = avlitree_node_get(tree->root, index);
	if(node == NULL)
		return NULL;

	return ((struct avlitree_ref_t *)((void *)node - offsetof(struct avlitree_ref_t, node)))->ref;
}

/**
 * Set the reference of the given index, returning the previous reference. No
 * reference will be added if the index does not exist.
 *   @tree: The AVL index tree.
 *   @index: The index.
 *   @ref: The reference.
 *   &returns: The referenceif found, 'NULL' if the given index does not
 *     exist.
 */

_export
void *avlitree_set(struct avlitree_t *tree, unsigned int index, void *ref)
{
	struct avlitree_node_t *node;
	struct avlitree_ref_t *value;

	value = mem_alloc(sizeof(struct avlitree_ref_t));
	value->ref = ref;

	node = avlitree_node_set(&tree->root, index, &value->node);
	if(node != NULL) {
		value = (void *)node - offsetof(struct avlitree_ref_t, node);
		ref = value->ref;
	}
	else
		ref = NULL;

	mem_free(value);

	return ref;
}


/**
 * Insert a reference into the AVL index tree.
 *   @tree: The AVL index tree.
 *   @index: The index.
 *   @ref: The reference.
 */

_export
void avlitree_insert(struct avlitree_t *tree, unsigned int index, void *ref)
{
	struct avlitree_ref_t *value;

	value = mem_alloc(sizeof(struct avlitree_ref_t));
	value->ref = ref;

	avlitree_node_insert(&tree->root, index, &value->node);
	tree->count++;
}

/**
 * Remove a reference at the given index.
 *   @tree: The AVL index tree.
 *   @index: The index.
 *   &returns: The value reference if found, 'NULL' otherwise.
 */

_export
void *avlitree_slice(struct avlitree_t *tree, unsigned int index)
{
	struct avlitree_node_t *node;

	node = avlitree_node_slice(&tree->root, index);
	if(node == NULL)
		return node;

	tree->count--;

	return ((struct avlitree_ref_t *)((void *)node - offsetof(struct avlitree_ref_t, node)))->ref;
}

/**
 * Remove and delete a reference from the AVL index tree.
 *   @tree: The AVL index tree.
 *   @index: The index.
 */

_export
void avlitree_erase(struct avlitree_t *tree, unsigned int index)
{
	void *ref;

	ref = avlitree_slice(tree, index);
	if(ref == NULL)
		throw("Index not found.");

	if(tree->delete != NULL)
		tree->delete(ref);
}

/**
 * Delete every reference from a tree, leaving it empty.
 *   @tree: The AVL index tree.
 *   &prop: noerror
 */

_export
void avlitree_clear(struct avlitree_t *tree)
{
	avlitree_node_clear(tree->root, ref_del, tree->delete);

	tree->root = NULL;
	tree->count = 0;
}


/**
 * Append a reference to the end of the AVL index tree. All indices of
 * existing elements will remain the same.
 *   @tree: The AVL index tree.
 *   @ref: The reference.
 */

_export
void avlitree_append(struct avlitree_t *tree, void *ref)
{
	avlitree_insert(tree, tree->count, ref);
}

/**
 * Add a reference to the beginning of the AVL index tree. All indices of
 * existing elements will be incremented by one.
 *   @tree: The AVL index tree.
 *   @ref: The reference.
 */

_export
void avlitree_prepend(struct avlitree_t *tree, void *ref)
{
	avlitree_insert(tree, 0, ref);
}


/**
 * Initialize an iterator on the AVL index tree.
 *   @tree: The AVL index tree.
 *   &returnS: The iterator.
 */

 _export
struct avlitree_iter_t avlitree_iter_new(const struct avlitree_t *tree)
{
	return avlitree_node_iter_new(tree->root);
}

/**
 * Retrieve the next reference from an AVL index tree iterator.
 *   @iter: The iterator.
 *   &returns: The next reference, 'NULL' if all references are exhausted.
 */

_export
void *avlitree_iter_next(struct avlitree_iter_t *iter)
{
	struct avlitree_node_t *node;

	node = avlitree_node_iter_next(iter);
	if(node == NULL)
		return NULL;

	return ((struct avlitree_ref_t *)((void *)node - offsetof(struct avlitree_ref_t, node)))->ref;
}

/**
 * Retrieve the next reference structure from an AVL index tree iterator.
 *   @iter: The iterator.
 *   &returns: The next key reference, 'NULL' if all references are exhausted.
 */

_export
struct avlitree_ref_t *avlitree_iter_next_ref(struct avlitree_iter_t *iter)
{
	struct avlitree_node_t *node;

	node = avlitree_node_iter_next(iter);
	if(node == NULL)
		return NULL;

	return (void *)node - offsetof(struct avlitree_ref_t, node);
}


/**
 * Create a new iterator over the references in an AVL index tree.
 *   @tree: The AVL index tree.
 *   &returns: The iterator.
 */

_export
struct iter_t avlitree_valiter_new(const struct avlitree_t *tree)
{
	struct iter_t iter;

	iter.ref = mem_alloc(sizeof(struct avlitree_iter_t));
	iter.iface = &iter_iface;
	*(struct avlitree_iter_t *)iter.ref = avlitree_iter_new(tree);

	return iter;
}

/**
 * Create a new iterator over the reference structures in an AVL index tree.
 *   @tree: The AVL index tree.
 *   &returns: The iterator.
 */

_export
struct iter_t avlitree_refiter_new(const struct avlitree_t *tree)
{
	struct iter_t iter;

	iter.ref = mem_alloc(sizeof(struct avlitree_iter_t));
	iter.iface = &refiter_iface;
	*(struct avlitree_iter_t *)iter.ref = avlitree_iter_new(tree);

	return iter;
}


/**
 * Delete a reference while clearing.
 *   @node: The node.
 *   @arg: The deletion callback.
 */

static void ref_del(struct avlitree_node_t *node, void *arg)
{
	delete_f delete = arg;
	struct avlitree_ref_t *ref = (void *)node - offsetof(struct avlitree_ref_t, node);

	if(delete != NULL)
		delete(ref->ref);

	mem_free(ref);
}


/**
 * Performs a single tree rotation of the given node. The node's child in the
 * opposite direction as the 'dir' paramter will replace itself as the parent,
 * placing the old parent as a child in the direction of the 'dir' parameter.
 * Wikipedia provides a good explanation with pictures.
 *   @node: The AVL index tree node.
 *   @dir: The direction to rotate, should be either the value 'LEFT' or
 *     'RIGHT'.
 *   &returns: The node that now takes the place of the node that was passed
 *     in.
 */

static struct avlitree_node_t *rotate_single(struct avlitree_node_t *node, uint8_t dir)
{
	struct avlitree_node_t *tmp;

	tmp = node->child[OTHERNODE(dir)];
	node->child[OTHERNODE(dir)] = tmp->child[dir];
	tmp->child[dir] = node;

	node->balance += NODEDIR(dir);
	if(NODEDIR(dir) * tmp->balance < 0)
		node->balance -= tmp->balance;

	tmp->balance += NODEDIR(dir);
	if(NODEDIR(dir) * node->balance > 0)
		tmp->balance += node->balance;

	tmp->parent = node->parent;
	node->parent = tmp;

	if(node->child[OTHERNODE(dir)] != NULL)
		node->child[OTHERNODE(dir)]->parent = node;

	recount(node);
	recount(tmp);

	return tmp;
}

/**
 * Performs a double rotation that is used to bring the grandchild to replace
 * its current position. Wikipedia provides a good explanation with pictures.
 *   @node: The AVL index tree node.
 *   @dir: The direction to rotate, should be either the value 'LEFT' or
 *     'RIGHT'.
 *   &returns: The node that now takes the place of the node that was passed
 *     in.
 */

static struct avlitree_node_t *rotate_double(struct avlitree_node_t *node, uint8_t dir)
{
	node->child[OTHERNODE(dir)] = rotate_single(node->child[OTHERNODE(dir)], OTHERNODE(dir));

	return rotate_single(node, dir);
}

/**
 * Recomputes the count for a given node based off of its child elements.
 *   @node: The node.
 *   &prop: noerror
 */

static void recount(struct avlitree_node_t *node)
{
	node->count = 1;

	if(node->child[LEFT] != NULL)
		node->count += node->child[LEFT]->count;

	if(node->child[RIGHT] != NULL)
		node->count += node->child[RIGHT]->count;
}
