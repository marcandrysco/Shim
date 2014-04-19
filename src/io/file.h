#ifndef IO_FILE_H
#define IO_FILE_H

/*
 * definitions
 */

#include "defs.h"


/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * file function declarations
 */

struct io_file_t _io_file_open(const char *path, enum io_file_e opt);
void io_file_close(struct io_file_t file);

bool io_file_ctrl(struct io_file_t file, unsigned int id, void *data);

size_t io_file_read(struct io_file_t file, void *restrict buf, size_t nbytes);
size_t io_file_write(struct io_file_t file, const void *restrict buf, size_t nbytes);

uint64_t io_file_tell(struct io_file_t file);
uint64_t io_file_seek(struct io_file_t file, int64_t offset, enum io_whence_e whence);

/*
 * i/o file function declarations
 */

struct io_input_t io_file_input(struct io_file_t file);
struct io_output_t io_file_output(struct io_file_t file);

/*
 * convenience definitions
 */

#ifdef _debug
//#	define io_file_open(path, opt) _io_file_open_dbg(path, opt, __FILE__, __LINE__)
#	define io_file_open(path, opt) _io_file_open(path, opt)
#else
#	define io_file_open(path, opt) _io_file_open(path, opt)
#endif

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
