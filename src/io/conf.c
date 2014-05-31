#include "../common.h"
#include "conf.h"
#include "../debug/exception.h"
#include "../mem/manage.h"
#include "../string/base.h"
#include "../string/locale.h"
#include "../types/avltree.h"
#include "../types/compare.h"
#include "input.h"


/**
 * Configuration structure.
 *   @id: The level identifier.
 *   @sub, pair: The sub-configuration and pair trees.
 */

struct io_conf_t {
	char *id;
	struct avltree_t sub, pair;
};


/*
 * local function declarations
 */

static int conf_cmp(const struct io_conf_t *left, const struct io_conf_t *right);

static bool chkend(const char **ptr);


/**
 * Create a new, blank configuration.
 *   &return: The configuration.
 */

_export
struct io_conf_t *io_conf_new()
{
	struct io_conf_t *conf;

	conf = mem_alloc(sizeof(struct io_conf_t));
	conf->id = NULL;
	conf->sub = avltree_empty((compare_f)conf_cmp, (delete_f)io_conf_delete);
	conf->pair = avltree_empty(compare_str, (delete_f)io_conf_pair_delete);

	return conf;
}

/**
 * Delete a configuration.
 *   @conf: The configuration.
 */

_export
void io_conf_delete(struct io_conf_t *conf)
{
	avltree_destroy(&conf->sub);
	avltree_destroy(&conf->pair);
	mem_delete(conf->id);
	mem_free(conf);
}


static char *readstr(const char **str)
{
	char *val;
	const char *ptr;

	if(chkend(str))
		return NULL;

	switch(**str) {
	case '"':
		ptr = ++(*str);
		while(*ptr != '"') {
			if(*ptr == '\0')
				throw("Unterminated quote.");

			ptr++;
		}

		val = str_ndup(*str, ptr++ - *str);

		break;

	case '\'':
		ptr = ++(*str);
		while(*ptr != '\'') {
			if(*ptr == '\0')
				throw("Unterminated quote.");

			ptr++;
		}

		val = str_ndup(*str, ptr++ - *str);

		break;

	default:
		ptr = *str;
		while(!str_isspace(*ptr) && (*ptr != '\0'))
			ptr++;

		val = str_ndup(*str, ptr - *str);
	}

	*str = ptr;

	return val;
}


/**
 * Read a configuration from an input.
 *   @input: The input.
 *   &returns: The configuration.
 */

_export
struct io_conf_t *io_conf_read(struct io_input_t input)
{
	char *line, *key, *value;
	const char *ptr;
	struct io_conf_t *conf;
	bool end = false;
	unsigned int n = 0;

	conf = io_conf_new();

	while((line = io_input_line(input)) != NULL) {
		n++;
		ptr = line;

		key = readstr(&ptr);
		if(key != NULL) {
			value = readstr(&ptr);
			if(value != NULL) {
				if(!chkend(&ptr))
					throw("%u: Extra characters at end of line.", n);

				if(str_isequal(key, "Begin")) {
					struct io_conf_t *sub;

					sub = io_conf_read(input);
					sub->id = value;
					avltree_insert(&conf->sub, sub, sub);
				}
				else {
					struct io_conf_pair_t *pair;

					pair = mem_alloc(sizeof(struct io_conf_pair_t));
					pair->key = key;
					pair->value = value;
					avltree_insert(&conf->pair, pair->key, pair);
				}
			}
			else {
				if(str_isequal(key, "Begin")) {
					struct io_conf_t *sub;

					sub = io_conf_read(input);
					avltree_insert(&conf->sub, sub, sub);
				}
				else if(str_isequal(key, "End"))
					end = true;
				else
					throw("Invalid directive '%s'.", key);

				mem_free(key);
			}
		}

		mem_free(line);

		if(end)
			break;
	}

	return conf;
}

/**
 * Load a configuration from a path.
 *   @path: The path.
 *   &returns: The configuration.
 */

_export
struct io_conf_t *io_conf_load(const char *path)
{
	struct io_conf_t *conf;
	struct io_input_t input;

	input = io_input_open(path);
	conf = io_conf_read(input);
	io_input_close(input);

	return conf;
}


/**
 * Lookup a value from the configuration.
 *   @conf: The configuration.
 *   @key: The key.
 *   &returns: The value.
 */

_export
const char *io_conf_value_lookup(struct io_conf_t *conf, const char *key)
{
	struct io_conf_pair_t *pair;

	pair = avltree_lookup(&conf->pair, key);
	return pair ? pair->value : NULL;
}

/**
 * Add a key-value pair to the configuration.
 *   @conf: The configuration.
 *   @key: The key.
 *   @value: The value.
 */

_export
void io_conf_value_add(struct io_conf_t *conf, const char *key, const char *value)
{
	struct io_conf_pair_t *pair;

	pair = mem_alloc(sizeof(struct io_conf_pair_t));
	pair->key = str_dup(key);
	pair->value = str_dup(value);
	avltree_insert(&conf->pair, pair->key, pair);
}

/**
 * Remove a key-value pair from the configuration.
 *   @conf: The configuration.
 */

_export
void io_conf_value_remove(struct io_conf_t *conf, const char *key)
{
	avltree_purge(&conf->pair, key);
}


/**
 * Delete a configuration pair.
 *   @pair: The pair.
 */

_export
void io_conf_pair_delete(struct io_conf_pair_t *pair)
{
	mem_free(pair->key);
	mem_free(pair->value);
	mem_free(pair);
}


/**
 * Begin an iterator over pairs in the configuration.
 *   @conf: The configuration.
 *   &returns: The iterator.
 */

_export
struct io_conf_iter_t io_conf_iter_begin(struct io_conf_t *conf)
{
	return (struct io_conf_iter_t){ avltree_iter_begin(&conf->pair) };
}

/**
 * Retrieve the next pair from the iterator.
 *   @iter: The iterator.
 *   &returns: The next pair or null.
 */

_export
const struct io_conf_pair_t *io_conf_iter_next(struct io_conf_iter_t *iter)
{
	return avltree_iter_next(&iter->iter);
}


/**
 * Begin an iterator over subconfigurations in the configuration.
 *   @conf: The configuration.
 *   &returns: The iterator.
 */

_export
struct io_conf_subiter_t io_conf_subiter_begin(struct io_conf_t *conf)
{
	return (struct io_conf_subiter_t){ avltree_iter_begin(&conf->sub) };
}

/**
 * Retrieve the next configuration from the iterator.
 *   @iter: The iterator.
 *   &returns: The next configuration or null.
 */

_export
struct io_conf_t *io_conf_subiter_next(struct io_conf_subiter_t *iter)
{
	return avltree_iter_next(&iter->iter);
}


/**
 * Compare two configurations.
 *   @left: The left configuration.
 *   @right: The right configuration.
 *   &returns: Their order.
 */

static int conf_cmp(const struct io_conf_t *left, const struct io_conf_t *right)
{
	if((left->id != NULL) && (right->id == NULL))
		return 1;
	else if((left->id == NULL) && (right->id != NULL))
		return -1;
	else if((left->id == NULL) && (right->id == NULL))
		return compare_ptr(left, right);
	else
		return str_cmp(left->id, right->id);
}


/**
 * Check if the pointer refers to the end of a line. If not, the pointer is
 * advanced to the next non-whitespace character.
 *   @ptr: The pointer reference.
 *   &returns: True if at the end of a line, false otherwise.
 */

static bool chkend(const char **ptr)
{
	*ptr = str_ltrimptr(*ptr);
	return (**ptr == '\0') || (**ptr == '#');
}
