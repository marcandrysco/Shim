#ifndef IO_OUTPUT_H
#define IO_OUTPUT_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * output function declarations
 */

struct io_output_t io_output_stdout();
struct io_output_t io_output_stderr();

struct io_output_t _io_output_open(const char *path);
struct io_output_t _io_output_open_dbg(const char *path, const char *file, unsigned int line);
struct io_output_t _io_output_append(const char *path);
struct io_output_t _io_output_append_dbg(const char *path, const char *file, unsigned int line);

size_t io_output_write(struct io_output_t output, const void *restrict buf, size_t nbytes);
void io_output_writefull(struct io_output_t output, const void *restrict buf, size_t nbytes);
bool io_output_ctrl(struct io_output_t output, unsigned int cmd, void *arg);
void io_output_close(struct io_output_t output);

void io_output_bool(struct io_output_t output, bool value);
void io_output_uint8(struct io_output_t output, uint8_t value);
void io_output_uint32(struct io_output_t output, uint32_t value);
void io_output_ch(struct io_output_t output, char ch);
void io_output_str(struct io_output_t output, const char *str);

/*
 * convenience definitions
 */

#define io_stdout io_output_stdout()
#define io_stderr io_output_stderr()

#ifdef _debug
#	define io_output_open(path) _io_output_open_dbg(path, __FILE__, __LINE__)
#	define io_output_append(path) _io_output_append_dbg(path, __FILE__, __LINE__)
#else
#	define io_output_open(path) _io_output_open(path)
#	define io_output_append(path) _io_output_append(path)
#endif

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
