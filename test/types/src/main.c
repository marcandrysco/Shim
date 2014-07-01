#include "common.h"


/**
 * AVL tree testing.
 *   &returns: True of success, false on failure.
 */

bool test_avltree()
{
	unsigned int i, list[8] = { 4, 2, 3, 7, 1, 0, 6, 5};
	struct avltree_node_t *root = NULL, *cur, node[10000];

	printf("testing avltree... ");

	{

		for(i = 0; i < 8; i++)
			avltree_node_insert(&root, &node[list[i]], (avltree_compare_nodenode_f)compare_ptr, NULL);

		for(i = 0, cur = avltree_node_first(root); i < 8; i++, cur = avltree_node_next(cur)) {
			if(cur != &node[i])
				return printf("failed\n"), false;
		}
	}

	{
		root = NULL;

		for(i = 0; i < 1000; i++)
			avltree_node_insert(&root, &node[i*443%1000], (avltree_compare_nodenode_f)compare_ptr, NULL);

		for(i = 0, cur = avltree_node_first(root); i < 1000; i++, cur = avltree_node_next(cur)) {
			if(cur != &node[i])
				return printf("failed\n"), false;
			else if((cur->balance < -1) || (cur->balance > 1))
				return printf("failed\n"), false;
		}
	}

	{
		root = NULL;

		for(i = 0; i < 10000; i++)
			avltree_node_insert(&root, &node[i*5333%10000], (avltree_compare_nodenode_f)compare_ptr, NULL);

		for(i = 10000 - 1, cur = avltree_node_last(root); i != UINT_MAX; i--, cur = avltree_node_prev(cur)) {
			if(cur != &node[i])
				return printf("failed\n"), false;
			else if((cur->balance < -1) || (cur->balance > 1))
				return printf("failed\n"), false;
		}

		for(i = 1; i < 10000; i++) {
			if(avltree_node_atmost(root, (void *)&node[i] - 1, (avltree_compare_nodekey_f)compare_ptr, NULL) != &node[i - 1])
				return printf("failed\n"), false;
		}

		for(i = 0; i < 10000 - 1; i++) {
			if(avltree_node_atleast(root, (void *)&node[i] + 1, (avltree_compare_nodekey_f)compare_ptr, NULL) != &node[i + 1])
				return printf("failed\n"), false;
		}
	}

	printf("okay\n");

	return true;
}

/**
 * Arbitrary integer testing.
 *   &returns: True of success, false on failure.
 */

bool test_integer()
{
	char *endptr, buf[32];
	struct integer_t *val;
	unsigned rem;

	printf("testing integer... ");

	val = integer_zero();
	integer_delete(val);

	val = integer_zero();

	integer_add_uint(&val, 58);
	if((val->len != 1) || (val->arr[0] != 58))
		return printf("failed\n"), false;

	integer_add_uint(&val, UINT_MAX);
	if((val->len != 2) || (val->arr[0] != 57) || (val->arr[1] != 1))
		return printf("failed\n"), false;

	integer_delete(val);

	val = integer_new(14);

	integer_mul_ushort(&val, 11);
	if((val->len != 1) || (val->arr[0] != 154))
		return printf("failed\n"), false;

	integer_mul_ushort(&val, 9261);
	if((val->len != 1) || (val->arr[0] != 1426194))
		return printf("failed\n"), false;

	integer_delete(val);

	val = integer_new(4194314);

	rem = integer_div_ushort(&val, 20460);
	if((rem != 14) || (val->len != 1) || (val->arr[0] != 205))
		return printf("failed\n"), false;

	rem = integer_div_ushort(&val, 3);
	if((rem != 1) || (val->len != 1) || (val->arr[0] != 68))
		return printf("failed\n"), false;

	integer_delete(val);

	val = integer_parse("123", &endptr);
	if((val->len != 1) || val->neg || (val->arr[0] != 123))
		return printf("failed\n"), false;
	integer_delete(val);

	val = integer_parse("-0x123", &endptr);
	if((val->len != 1) || !val->neg || (val->arr[0] != 0x123))
		return printf("failed\n"), false;
	integer_delete(val);

	val = integer_new(78910);
	str_printf(buf, "%C", integer_chunk(val));
	integer_delete(val);
	if(!str_isequal(buf, "78910"))
		return printf("failed [%s]\n", buf), false;

	printf("okay\n");

	return true;
}

/**
 * Main entry point.
 *   @argc: The number of arguments.
 *   @argv: The argument array.
 *   &returns: The return status code.
 */

int main(int argc, char *argv[])
{
	bool suc = true;

	suc &= test_avltree();
	suc &= test_integer();

	return suc ? 0 : 1;
}
