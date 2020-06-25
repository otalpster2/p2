#include <stdio.h>

#include "errors.h"

int error_code = OK;

char* str_error_code(int error_code)
{
	if (error_code == OK)
		return "Success";

	// Related to command line arguments.
	else if (error_code == CLI_ARGS_INV_NUM)
		return "Invalid number of cli arguments";
	else if (error_code == CLI_ARGS_INV_TOKEN)
		return "Invalid token in cli arguments";

	// Related to 'input_dir' parsing.
	else if (error_code == DIRENT_INV_TYPE)
		return "Invalid subdirectory type";

	else
		{ error_code = INVALID_CODE; return NULL; }
}

void print_error(char str[])
{
	fprintf(stderr, "%s: %s.\n", str, str_error_code(error_code));
}
