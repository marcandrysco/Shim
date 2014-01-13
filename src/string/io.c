#include "../common.h"
#include "io.h"
#include "../io/output.h"
#include "../io/print.h"
#include "../mem/base.h"
#include "../mem/manage.h"


/**
 * Buffer structure.
 *   @buf: The internal data.
 *   @nbytes, nread: The number of bytes remaining and received.
 */

struct buf_t {
	char *buf;
	size_t nbytes, *nread;
};

/**
 * Buffer structure.
 *   @buf: The internal data.
 *   @i, nbytes, nread: The index, number of bytes remaining, and received.
 */

struct accum_t {
	char **buf;
	size_t i, nbytes, *nread;
};


/*
 * local function declarations
 */

static void buf_delete(struct buf_t *inst);
static size_t buf_write(struct buf_t *inst, const void *restrict buf, size_t nbytes);

static size_t len_write(size_t *len, const void *restrict buf, size_t nbytes);

static void accum_delete(struct accum_t *inst);
static size_t accum_write(struct accum_t *inst, const void *restrict buf, size_t nbytes);

static bool unimpl_ctrl(void *ref, unsigned int id, void *data);
static void unimpl_close(void *ref);

/*
 * local variables
 */

static struct io_output_i buf_iface = { { unimpl_ctrl, (io_close_f)buf_delete }, (io_write_f)buf_write };
static struct io_output_i len_iface = { { unimpl_ctrl, unimpl_close }, (io_write_f)len_write };
static struct io_output_i accum_iface = { { unimpl_ctrl, (io_close_f)accum_delete }, (io_write_f)accum_write };


/**
 * Create an output to a buffer.
 *   @buf: The buffer.
 *   @nbytes: The size of the buffer.
 *   @nread: Optional. The number of bytes received, may be larger than buffer.
 *   &returns: The output.
 */

_export
struct io_output_t str_output_buf(char *buf, size_t nbytes, size_t *nread)
{
	struct buf_t *inst;

	inst = mem_alloc(sizeof(struct buf_t));
	*inst = (struct buf_t){ buf, nbytes - 1, nread };

	return (struct io_output_t){ inst, &buf_iface };
}

/**
 * Delete the buffer instance.
 *   @inst: The buffer instance.
 */

static void buf_delete(struct buf_t *inst)
{
	*inst->buf = '\0';
	mem_free(inst);
}

/**
 * Write data into the buffer instance.
 *   @inst: The buffer instance.
 *   @buf: The input buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes received.
 */

static size_t buf_write(struct buf_t *inst, const void *restrict buf, size_t nbytes)
{
	if(inst->nread != NULL)
		*inst->nread += nbytes;

	if(inst->nbytes > nbytes) {
		mem_copy(inst->buf, buf, nbytes);
		inst->buf += nbytes;
		inst->nbytes -= nbytes;
	}
	else {
		mem_copy(inst->buf, buf, inst->nbytes);
		inst->nbytes = 0;
	}

	return nbytes;
}


/**
 * Create an output to a length reference.
 *   @len: The length reference.
 *   &returns: The output.
 */

_export
struct io_output_t str_output_len(size_t *len)
{
	*len = 0;

	return(struct io_output_t){ len, &len_iface };
}

/**
 * Write data into the length reference.
 *   @len: The length reference.
 *   @buf: The input buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes received.
 */

static size_t len_write(size_t *len, const void *restrict buf, size_t nbytes)
{
	*len += nbytes;

	return nbytes;
}


/**
 * Create an output to an accumulated buffer..
 *   @buf: The buffer reference.
 *   @nread: Optional. The number of bytes received, may be larger than buffer.
 *   &returns: The output.
 */

_export
struct io_output_t str_output_accum(char **buf, size_t *nread)
{
	struct accum_t *inst;

	inst = mem_alloc(sizeof(struct accum_t));
	inst->i = 0;
	inst->buf = buf;
	inst->nbytes = 32;
	inst->nbytes = 32;

	*inst->buf = mem_alloc(32);
	
	return (struct io_output_t){ inst, &accum_iface };
}

/**
 * Delete the accumulated buffer.
 *   @inst: The accumulated buffer instance.
 */

static void accum_delete(struct accum_t *inst)
{
	*inst->buf = mem_realloc(*inst->buf, inst->i + 1);
	(*inst->buf)[inst->i] = '\0';

	mem_free(inst);
}

/**
 * Write data into the accumulated buffer reference.
 *   @inst: The accumulated buffer instance.
 *   @buf: The input buffer.
 *   @nbytes: The number of bytes.
 *   &returns: The number of bytes received.
 */

static size_t accum_write(struct accum_t *inst, const void *restrict buf, size_t nbytes)
{
	if(inst->nread != NULL)
		*inst->nread += nbytes;

	if((inst->i + nbytes) > inst->nbytes) {
		inst->nbytes = 2 * (inst->i + nbytes);
		*inst->buf = mem_realloc(*inst->buf, inst->nbytes);
	}

	mem_copy(*inst->buf + inst->i, buf, nbytes);
	inst->i += nbytes;

	return nbytes;
}


/**
 * Unimplemented control handler.
 *   @ref: The reference.
 *   @id: The control ID.
 *   @data: The data.
 *   &returns: Always false.
 */

static bool unimpl_ctrl(void *ref, unsigned int id, void *data)
{
	return false;
}

/**
 * Unimplemented close handler.
 *   @ref: The reference.
 */

static void unimpl_close(void *ref)
{
}


/**
 * Print formatted data to a string.
 *   @buf: The buffer.
 *   @format: The print-style format.
 *   @...: The print-style arguments.
 */

_export
void str_printf(char *buf, const char *restrict format, ...)
{
	va_list args;

	va_start(args, format);
	str_vprintf(buf, format, args);
	va_end(args);
}

/**
 * Print formatted data to a string using a variable argument list.
 *   @buf: The buffer.
 *   @format: The print-style format.
 *   @args: The print-style arguments.
 */

_export
void str_vprintf(char *buf, const char *restrict format, va_list args)
{
	struct io_output_t output;

	output = str_output_buf(buf, SIZE_MAX, NULL);
	io_vprintf(output, format, args);
	io_output_close(output);
}


/**
 * Print formatted data to a string.
 *   @buf: The buffer.
 *   @nbytes: The buffer size.
 *   @format: The print-style format.
 *   @...: The print-style arguments.
 *   &returns: The length of the string given a large enough buffer.
 */

_export
size_t str_nprintf(char *buf, size_t nbytes, const char *restrict format, ...)
{
	size_t len;
	va_list args;

	va_start(args, format);
	len = str_vnprintf(buf, nbytes, format, args);
	va_end(args);

	return len;
}

/**
 * Print formatted data to a string using a variable argument list.
 *   @buf: The buffer.
 *   @nbytes: The buffer size.
 *   @format: The print-style format.
 *   @args: The print-style arguments.
 *   &returns: The length of the string given a large enough buffer.
 */

_export
size_t str_vnprintf(char *buf, size_t nbytes, const char *restrict format, va_list args)
{
	size_t len;
	struct io_output_t output;

	output = str_output_buf(buf, nbytes, &len);
	io_vprintf(output, format, args);
	io_output_close(output);

	return len;
}


/**
 * Print formatted data to determine its length.
 *   @buf: The buffer.
 *   @nbytes: The buffer size.
 *   @format: The print-style format.
 *   @...: The print-style arguments.
 *   &returns: The length of the output string.
 */

_export
size_t str_lprintf(const char *restrict format, ...)
{
	va_list args;
	size_t nbytes;

	va_start(args, format);
	nbytes = str_vlprintf(format, args);
	va_end(args);

	return nbytes;
}

/**
 * Print formatted data to determine its length using a variable argument list.
 *   @buf: The buffer.
 *   @nbytes: The buffer size.
 *   @format: The print-style format.
 *   @args: The print-style arguments.
 *   &returns: The length of the output string.
 */

_export
size_t str_vlprintf(const char *restrict format, va_list args)
{
	struct io_output_t output;
	size_t nbytes;

	output = str_output_len(&nbytes);
	io_vprintf(output, format, args);
	io_output_close(output);

	return nbytes;
}


/**
 * Print formatted data to an allocated string.
 *   @format: The print-style format.
 *   @...: The print-style arguments.
 *   &returns: The allocated string.
 */

_export
char *str_aprintf(const char *restrict format, ...)
{
	va_list args;
	char *accum;

	va_start(args, format);
	accum = str_vaprintf(format, args);
	va_end(args);

	return accum;
}

/**
 * Print formatted data to an allocated string using a variable argument list.
 *   @format: The print-style format.
 *   @args: The print-style arguments.
 *   &returns: The allocated string.
 */

_export
char *str_vaprintf(const char *restrict format, va_list args)
{
	char *accum;
	struct io_output_t output;

	output = str_output_accum(&accum, NULL);
	io_vprintf(output, format, args);
	io_output_close(output);

	return accum;
}
