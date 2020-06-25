#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "cli_args/cli_args.h"
#include "errors/errors.h"
#include "read_write/read_write.h"

char** read_subdirs(int fd_r, unsigned* n_subdirs)
{
	size_t size;
	read_size(fd_r, &size);

	char** subdirs = NULL;
	if ((subdirs = malloc(sizeof(char*) * size)) == NULL)
		{ perror("Worker: Malloc subdirs"); exit(errno); }

	for (unsigned c_subdir = 0; c_subdir < size; ++c_subdir)
		read_data(fd_r, subdirs + c_subdir);

	*n_subdirs = size;
	return subdirs;
}

int main(int argc, char* argv[])
{
	printf("Child: Hello! I'm a child.\n");
	sleep(5);
	printf("Printing arguments:\n");
	for (int i = 0; i < argc; ++i)
		printf("%s\n", argv[i]);

	struct cli_args* args = NULL;
	if ((args = parse_cli_args(argc, argv)) == NULL)
		{ print_error("Failed to parse cli arguments"); return error_code; }

	sleep(5);
	printf("Child: Opening FIFO.\n");
	int r_fd;
	if ((r_fd = open(args->fifo_read, O_RDONLY)) < 0)
		{ perror("Client: can't open read FIFO"); exit(errno); }
	//int w_fd;
	//if ((w_fd = open(args->fifo_write, O_WRONLY)) < 0)
	//	{ perror("Client: can't open write FIFO"); exit(errno); }
	printf("Child: Opened FIFO.\n");

	char* buffer = NULL;
	read_data(r_fd, &buffer);
	printf("Child: I read: %s.\n", buffer);

	printf("Worker: Reading subdirs.\n");
	unsigned n_subdirs;     // The number of subdirectory paths.
	char** subdirs = NULL;  // Where subdirectory paths will be stored.
	subdirs = read_subdirs(r_fd, &n_subdirs);
	printf("Worker: Read subdirs.\n");
	
	char str[256];
	sprintf(str, "Worker with fifo: %s", args->fifo_read);
	for (unsigned c_subdir = 0; c_subdir < n_subdirs; ++c_subdir)
		printf("%s: %s.\n", str, subdirs[c_subdir]);

	unsigned port = read_server_port(r_fd);
	char* server_ip = read_server_ip(r_fd);

	close(r_fd);
	//close(w_fd);

	return 0;
}
