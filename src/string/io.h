#ifndef STRING_IO_H
#define STRING_IO_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * string output function declarations
 */

struct io_output_t str_output_buf(char *buf, size_t nbytes, size_t *nread);
struct io_output_t str_output_len(size_t *len);
struct io_output_t str_output_accum(char **buf, size_t *nread);

/*
 * string formatted printing funtcion declarations
 */

void str_printf(char *buf, const char *restrict format, ...);
void str_vprintf(char *buf, const char *restrict format, va_list args);

size_t str_nprintf(char *buf, size_t nbytes, const char *restrict format, ...);
size_t str_vnprintf(char *buf, size_t nbytes, const char *restrict format, va_list args);

size_t str_lprintf(const char *restrict format, ...);
size_t str_vlprintf(const char *restrict format, va_list args);

char *str_aprintf(const char *restrict format, ...);
char *str_vaprintf(const char *restrict format, va_list args);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
