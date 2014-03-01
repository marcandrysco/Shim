#ifndef FS_BASE_H
#define FS_BASE_H

/*
 * start header: shim.h
 */

/* %shim.h% */

/*
 * base filesystem function declarations
 */

bool fs_exists(const char *path);
bool fs_isfile(const char *path);
bool fs_islink(const char *path);
bool fs_isdir(const char *path);

void fs_write(const char *path, const void *buf, size_t nbytes);
void fs_writestr(const char *path, const char *str);
void fs_read(const char *path, void *buf, size_t nbytes);
char *fs_readstr(const char *path);
size_t fs_copy(const char *dest, const char *src);

void fs_link(const char *dest, const char *target);
bool fs_trylink(const char *dest, const char *target);
void fs_symlink(const char *dest, const char *target);
bool fs_trysymlink(const char *dest, const char *target);
void fs_mkdir(const char *path);

void fs_rmdir(const char *path);
void fs_rmfile(const char *path);
void fs_clear(const char *path);

struct iter_t fs_lsdir(const char *path);

char *fs_mktmpdir(const char *prefix);

/* %~shim.h% */

/*
 * end header: shim.h
 */

#endif
