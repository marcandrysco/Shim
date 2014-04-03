#ifndef IO_INPUT_H
#define IO_INPUT_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * input function declarations
 */

struct io_input_t io_input_stdin();

struct io_input_t _io_input_open(const char *path);
struct io_input_t _io_input_open_dbg(const char *path, const char *file, unsigned int line);

bool io_input_ctrl(struct io_input_t input, unsigned int cmd, void *arg);
size_t io_input_read(struct io_input_t input, void *restrict buf, size_t nbytes);
void io_input_readfull(struct io_input_t input, void *restrict buf, size_t nbytes);
int16_t io_input_byte(struct io_input_t input);
void io_input_close(struct io_input_t input);

bool io_input_eos(struct io_input_t input);

bool io_input_bool(struct io_input_t input);
uint8_t io_input_uint8(struct io_input_t input);
uint16_t io_input_uint16(struct io_input_t input);
uint32_t io_input_uint32(struct io_input_t input);
uint64_t io_input_uint64(struct io_input_t input);
char io_input_ch(struct io_input_t input);
char *io_input_str(struct io_input_t input);
char *io_input_strptr(struct io_input_t input);
char *io_input_line(struct io_input_t input);
float io_input_float(struct io_input_t input);
double io_input_double(struct io_input_t input);

/*
 * convenience definitions
 */

#define io_stdin io_input_stdin()

#ifdef _debug
#	define io_input_open(path) _io_input_open_dbg(path, __FILE__, __LINE__)
#else
#	define io_input_open(path) _io_input_open(path)
#endif

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
