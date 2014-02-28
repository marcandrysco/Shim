#include "common.h"


/**
 * Directory testing
 *   &returns: True of success, false on failure.
 */

bool test_fs_dir()
{
	char *path;

	printf("testing directories... ");

	fs_mkdir("./tmp-test");
	if(!fs_isdir("./tmp-test"))
		return printf("failed\n"), false;

	fs_rmdir("./tmp-test");

	path = fs_mktmpdir("./tmp-");
	if(!fs_isdir(path))
		return printf("failed\n"), false;

	fs_rmdir(path);
	mem_free(path);

	printf("okay\n");

	return true;
}

/**
 * Path testing
 *   &returns: True of success, false on failure.
 */

bool test_fs_path()
{
	printf("testing paths... ");

	if(!str_isequal(fs_basename("./file"), "file"))
		return printf("failed\n"), false;

	if(!str_isequal(fs_basename("file"), "file"))
		return printf("failed\n"), false;

	if(!str_isequal(fs_basename("/usr/local/bin"), "bin"))
		return printf("failed\n"), false;

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

	suc &= test_fs_dir();
	suc &= test_fs_path();

	return suc ? 0 : 1;
}
