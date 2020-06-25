#ifndef ERRORS_H
#define ERRORS_H

enum errors {
	OK,  // Successful execution.

	// Related to command line arguments.
	CLI_ARGS_INV_NUM,    // Invalid number of cli arguments.
	CLI_ARGS_INV_TOKEN,  // Invalid flag in cli arguments.

	DIRENT_INV_TYPE,  // Invalid type of subdirectory entry.

	INVALID_CODE  // Invalid error code passed into 'str_error_code()'.
};

extern int error_code;

/*
 * Returns a human-readable string describing the provided error code.
 * If you provide 'error_code' as parameter, then you will get the execution
 * status of the last instruction.
 *
 * The returned string is immutable.
 *
 * Success: Returns human-readable string describing provided error code.
 * Failure: Returns NULL and sets 'error_code'.
 */
char* str_error_code(int error_code);

/*
 * Prints to stderr a human-readable message about the execution status of the
 * last instruction in human-readable form.
 */
void print_error(char str[]);

#endif  // #ifndef ERRORS_H
