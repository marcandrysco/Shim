#include "../common.h"
#include "conf.h"
#include "../debug/exception.h"
#include "../mem/manage.h"
#include "../string/base.h"
#include "../string/locale.h"
#include "../types/avlitree.h"
#include "../types/avltree.h"
#include "../types/compare.h"
#include "input.h"


/**
 * Configuration structure.
 *   @id: The identifier.
 *   @sub: The sub-configuration.
 *   @pair: The pairs in order.
 *   @accum: The accumulated values by id.
 */

struct io_conf_t {
	char *id;
	struct avlitree_t sub, pair;
	struct avltree_t accum;
};


/*
 * local function declarations
 */

static char *readstr(const char **str);
static bool chkend(const char **ptr);


/**
 * Create a new, blank configuration.
 *   &returns: The configuration.
 */

_export
struct io_conf_t *io_conf_new()
{
	struct io_conf_t *conf;

	conf = mem_alloc(sizeof(struct io_conf_t));
	conf->id = NULL;
	conf->sub = avlitree_empty((delete_f)io_conf_delete);
	conf->pair = avlitree_empty((delete_f)io_conf_pair_delete);
	conf->accum = avltree_empty(compare_str, (delete_f)io_conf_pair_delete);

	return conf;
}

/**
 * Delete a configuration.
 *   @conf: The configuration.
 */

_export
void io_conf_delete(struct io_conf_t *conf)
{
	avlitree_destroy(&conf->sub);
	avlitree_destroy(&conf->pair);
	avltree_destroy(&conf->accum);
	mem_delete(conf->id);
	mem_free(conf);
}


/**
 * Read a configuration from an input.
 *   @input: The input.
 *   &returns: The configuration.
 */

_export
struct io_conf_t *io_conf_read(struct io_input_t input)
{
	unsigned int n;
	bool term = false;
	char *line, *key, *value;
	const char *ptr;
	struct io_conf_t *conf, *sub;
	struct io_conf_pair_t *pair, *accum;

	conf = io_conf_new();
	for(n = 1; ((line = io_input_line(input)) != NULL); n++) {
		ptr = line;

		key = readstr(&ptr);
		if(key != NULL) {
			if(str_isequal(key, "Begin")) {
				sub = io_conf_read(input);
				sub->id = readstr(&ptr);
				avlitree_append(&conf->sub, sub);
			}
			else if(str_isequal(key, "End"))
				term = true;
			else {
				pair = io_conf_pair_new(key);
				avlitree_append(&conf->pair, pair);

				accum = avltree_lookup(&conf->accum, key);
				if(accum == NULL) {
					accum = io_conf_pair_new(key);
					avltree_insert(&conf->accum, accum->id, accum);
				}

				while((value = readstr(&ptr)) != NULL) {
					io_conf_pair_add(pair, value);
					io_conf_pair_add(accum, value);
					mem_free(value);
				}
			}

			if(!chkend(&ptr))
				throw("Unexpected data at end of line %u.", n);

			mem_free(key);
		}

		mem_free(line);

		if(term)
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
 * Retrieve the configuration identifier.
 *   @conf: The configuration.
 *   &returns: The identifier or null.
 */

_export
const char *io_conf_id(struct io_conf_t *conf)
{
	return conf->id;
}

/**
 * Retrieve the number of pairs accumulated by key.
 *   @conf: The configuration.
 *   &returns: The number of pairs.
 */

_export
unsigned int io_conf_accum(struct io_conf_t *conf)
{
	return conf->accum.count;
}


/**
 * Lookup a pair from a configuration.
 *   @conf: The configuration.
 *   @id: The identifier.
 *   &returns: The configuration pair.
 */

_export
struct io_conf_pair_t *io_conf_lookup(struct io_conf_t *conf, const char *id)
{
	return avltree_lookup(&conf->accum, id);
}

/**
 * Lookup a single value with a given ID, throwing an error if multiple values
 * are set.
 *   @conf: The configuration.
 *   @id: The identifier.
 *   &returns: The value or null if no such key exists.
 */

_export
const char *io_conf_lookup_single(struct io_conf_t *conf, const char *id)
{
	struct io_conf_pair_t *pair;

	pair = io_conf_lookup(conf, id);
	if(pair == NULL)
		return NULL;
	else if(pair->nvalues != 1)
		throw("Too many values with key '%s'.", pair->id);
	else
		return pair->value[0];
}

/**
 * Lookup the array of values for a given ID.
 *   @conf: The configuration.
 *   @id: The identifier.
 *   &returns: The value array or null.
 */

_export
const char *const *io_conf_lookup_values(struct io_conf_t *conf, const char *id)
{
	struct io_conf_pair_t *pair;

	pair = io_conf_lookup(conf, id);
	return pair ? (const char *const *)pair->value : NULL;
}

/**
 * Remove a pair from the configuration. The pair is still available through
 * iteration.
 *   @conf: The configuration.
 *   @id: The identifier.
 *   &returns: The configuration pair.
 */

_export
struct io_conf_pair_t *io_conf_remove(struct io_conf_t *conf, const char *id)
{
	return avltree_remove(&conf->accum, id);
}

/**
 * Remove a single value with a given ID, throwing an error if multiple values
 * are set. The returned value must be freed.
 *   @conf: The configuration.
 *   @id: The identifier.
 *   &returns: The value or null if no such key exists.
 */

_export
char *io_conf_remove_single(struct io_conf_t *conf, const char *id)
{
	char *value;
	struct io_conf_pair_t *pair;

	pair = io_conf_lookup(conf, id);
	if(pair == NULL)
		return NULL;
	else if(pair->nvalues != 1)
		throw("Too many values with key '%s'.", pair->id);

	value = pair->value[0];
	pair->value[0] = NULL;

	io_conf_pair_delete(pair);

	return value;
}


/**
 * Begin an iterator over pairs in the configuration.
 *   @conf: The configuration.
 *   &returns: The iterator.
 */

_export
struct io_conf_iter_t io_conf_iter_begin(struct io_conf_t *conf)
{
	return (struct io_conf_iter_t){ avlitree_iter_begin(&conf->pair) };
}

/**
 * Retrieve the next pair from the iterator.
 *   @iter: The iterator.
 *   &returns: The next pair or null.
 */

_export
const struct io_conf_pair_t *io_conf_iter_next(struct io_conf_iter_t *iter)
{
	return avlitree_iter_next(&iter->inner);
}


/**
 * Begin an iterator over subconfigurations in the configuration.
 *   @conf: The configuration.
 *   &returns: The iterator.
 */
_export
struct io_conf_subiter_t io_conf_subiter_begin(struct io_conf_t *conf)
{
	return (struct io_conf_subiter_t){ avlitree_iter_begin(&conf->sub) };
}

/**
 * Retrieve the next configuration from the iterator.
 *   @iter: The iterator.
 *   &returns: The next configuration or null.
 */

_export
struct io_conf_t *io_conf_subiter_next(struct io_conf_subiter_t *iter)
{
	return avlitree_iter_next(&iter->inner);
}


/**
 * Create an empty configuration pair.
 *   @id: The identifier.
 *   &returns: The pair.
 */

_export
struct io_conf_pair_t *io_conf_pair_new(const char *id)
{
	struct io_conf_pair_t *pair;

	pair = mem_alloc(sizeof(struct io_conf_pair_t));
	pair->id = str_dup(id);
	pair->nvalues = 0;
	pair->value = mem_alloc(sizeof(char *));
	pair->value[0] = NULL;

	return pair;
}

/**
 * Delete a configuration pair.
 *   @pair: The pair.
 */

_export
void io_conf_pair_delete(struct io_conf_pair_t *pair)
{
	char **value;

	for(value = pair->value; *value != NULL; value++)
		mem_free(*value);

	mem_free(pair->value);
	mem_free(pair->id);
	mem_free(pair);
}

/**
 * Add a value to the pair.
 *   @pair: The pair.
 *   @value: The value.
 */

_export
void io_conf_pair_add(struct io_conf_pair_t *pair, const char *value)
{
	pair->value = mem_realloc(pair->value, (pair->nvalues + 2) * sizeof(char *));
	pair->value[pair->nvalues++] = str_dup(value);
	pair->value[pair->nvalues] = NULL;
}


/**
 * Read a single string from the line.
 *   @str: The origin string.
 *   &returns: The extracted string.
 */

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
