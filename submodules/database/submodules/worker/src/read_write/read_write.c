#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

int read_size(int fd_r, size_t* size)
{
	size_t length = sizeof(size_t);
	char* bytes = NULL;
	if ((bytes = malloc(sizeof(char) * length)) == NULL)
		{ perror("Master: Malloc read bytes"); exit(errno); }

	size_t rbytes = 0;
	while (rbytes < length) {
		ssize_t rv;
		if ((rv = read(fd_r, bytes + rbytes, 1)) < 0)
			{ perror("Master: Read size"); exit(errno); }
		rbytes += rv;
	}

	*size = *(size_t*)bytes;
	free(bytes);

	return rbytes;
}

int write_size(int fd_w, size_t size)
{
	size_t length = sizeof(size_t);
	char* bytes = NULL;
	if ((bytes = malloc(sizeof(char) * length)) == NULL)
		{ perror("Master: Malloc write bytes"); exit(errno); }

	*(size_t*)bytes = size;

	size_t wbytes = 0;
	while(wbytes < length) {
		ssize_t rv;
		if ((rv = write(fd_w, bytes + wbytes, 1)) < 0)
			{ perror("Master: Write size"); exit(errno); }
		wbytes += rv;
	}

	free(bytes);

	return wbytes;
}

int read_data(int fd_r, char** buffer)
{
	/*
	 * Read the length of the incomming message.
	 * Messages also contain the NULL terminating byte.
	 */
	size_t size;
	read_size(fd_r, &size);

	*buffer = NULL;
	if ((*buffer = malloc(sizeof(char) * size)) == NULL)
		{ perror("Master: Malloc read buffer"); exit(errno); }

	size_t rbytes = 0;
	while (rbytes < size) {
		ssize_t rv;
		if ((rv = read(fd_r, (*buffer) + rbytes, 1)) < 0)
			{ perror("Master: read buffer"); exit(errno); }
		rbytes += rv;
	}

	return rbytes;
}

int write_data(int fd_w, char* buffer)
{
	size_t length = strlen(buffer) + 1;
	write_size(fd_w, length);

	size_t wbytes = 0;
	while (wbytes < length) {
		ssize_t rv;
		if ((rv = read(fd_w, buffer + wbytes, 1)) < 0)
			{ perror("Master: write buffer"); exit(errno); }
		wbytes += rv;
	}

	return wbytes;
}
