#include "common.h"


/*
 * local function declasrations
 */

static void atexit_handler();


/**
 * Exit handler test.
 *   &returns: True of success, false on failure.
 */

bool test_atexit()
{
	printf("testing exit handler... ");
	sys_atexit(atexit_handler);
	printf("okay\n");

	return true;
}

/**
 * Exit handler.
 */

static void atexit_handler()
{
	printf("!atexit exec\n");
}

/**
 * Exception test.
 *   &returns: True of success, false on failure.
 */

bool test_exception()
{
	printf("testing exceptions... ");

	try { }

	try { }
	catch(e) { }

	try
		throw("errmsg1");
	catch(e) {
		if(!str_isequal(e, "errmsg1"))
			return printf("failed\n"), false;
	}

	try
		throw("errmsg2");
	catch(e) {
		try
			throw(e);
		catch(e) {
			if(!str_isequal(e, "errmsg2"))
				return printf("failed\n"), false;
		}
	}

	try {
		try
			throw("errmsg3");
		catch(e)
			throw(e);
	}
	catch(e) {
		if(!str_isequal(e, "errmsg3"))
			return printf("failed\n"), false;
	}

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

	suc &= test_exception();
	suc &= test_atexit();

	return suc ? 0 : 1;
}
