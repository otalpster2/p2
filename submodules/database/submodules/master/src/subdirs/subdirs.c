#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "../read_write/read_write.h"
#include "../worker/worker.h"
#include "../errors/errors.h"

char** parse_subdirs(char* base_path, unsigned* n_dirs)
{
	DIR* dir = NULL;
	if ((dir = opendir(base_path)) == NULL)
		{ perror("Master: Open directory input dir"); exit(errno); }

	char** subdirs;
	*n_dirs = 8;  // We start with space for 8 subdirectories.
	if ((subdirs = malloc(sizeof(char*) * (*n_dirs))) == NULL)
		{ perror("Master: Malloc for subdirs paths"); exit(errno); }

	unsigned next = 0;  // The next free index in 'subdirs'.
	struct dirent* dirent = NULL;
	while ((dirent = readdir(dir)) != NULL) {
		if (!strcmp(dirent->d_name, ".")) continue;
		if (!strcmp(dirent->d_name, "..")) continue;
		// Calculate relative path appropriate size.
		char* relative_path = NULL;
		size_t len = strlen(base_path);
		if (base_path[len - 1] == '/')
			base_path[--len] = '\0';
		len += strlen(dirent->d_name);
		len += 2;  // One for NULL byte, one for front slash.
		relative_path = malloc(sizeof(char) * len);
		// Forge the 'base_path' and dirent name into 'relative path'.
		strcpy(relative_path, base_path);
		strcat(relative_path, "/");
		strcat(relative_path, dirent->d_name);

		struct stat statbuf;
		if (stat(relative_path, &statbuf) < 0)
			{ perror("Master: Can't stat dirent"); exit(errno); }
		if ((statbuf.st_mode & S_IFMT) != S_IFDIR)
			{ error_code = DIRENT_INV_TYPE; return NULL; }

		if (next == (*n_dirs)) {
			*n_dirs *= 2;
			subdirs = realloc(subdirs, sizeof(char*) * (*n_dirs));
		}

		subdirs[next] =
			malloc(sizeof(char) * (strlen(relative_path) + 1));
		strcpy(subdirs[next++], relative_path);

		free(relative_path);
	}
	closedir(dir);

	*n_dirs = next;
	return subdirs;
}

char*** split_subdirs(char** subdirs, unsigned n_subdirs,
		unsigned n_workers, unsigned** n_worker_subdirs)
{
	char*** splitted_subdirs = NULL;
	if ((splitted_subdirs = malloc(sizeof(char **) * n_workers)) == NULL)
		{ perror("Master: Malloc for split"); exit(errno); }

	*n_worker_subdirs = NULL;
	if ((*n_worker_subdirs = malloc(sizeof(unsigned) * n_workers)) == NULL)
		{ perror("Master: Malloc for n_split"); exit(errno); }
	for (unsigned i = 0; i < n_workers; (*n_worker_subdirs)[i++] = 0);

	// We start with 4 directories space for each worker.
	unsigned allocated = 4;
	for (unsigned c_worker = 0; c_worker < n_workers; ++c_worker)
		if ((splitted_subdirs[c_worker] =
				malloc(sizeof(char*) * allocated)) == NULL) {
			perror("Master: Malloc worker's subdirs");
			exit(errno);
		}

	// Give one subdir to each worker in a rotating manner.
	for (unsigned c_subdir = 0, c_worker = 0;
			c_subdir < n_subdirs;
			++c_subdir,
			++c_worker, c_worker %= n_workers) {
		/*
		 * The next free pointer depends on the number of rotations
		 * we've performed. Remember that we rotate through each worker
		 * and assign a subdirectory.
		 */
		unsigned next = c_subdir / n_workers;

		/*
		 * Remember that we store the number of subdirs of each worker
		 * into 'n_worker_subdirs' array. 'next' is the index of the
		 * next free pointer, which we assign to point to an element.
		 * Then we proceed to the next worker.
		 * Therefore we need the '+1'.
		 */
		(*n_worker_subdirs)[c_worker] = next + 1;

		if (next == allocated) {
			allocated *= 2;
			for (unsigned c_worker = 0; c_worker < n_workers; ++c_worker) {
				if ((splitted_subdirs[c_worker] =
						realloc(splitted_subdirs[c_worker],
						sizeof(char*) * allocated)) == NULL) {
					perror("Master: Realloc worker's subdirs");
					exit(errno);
				}
			}
		}

		splitted_subdirs[c_worker][next] = subdirs[c_subdir];
	}

	return splitted_subdirs;
}

void send_subdirs(struct worker** workers, unsigned n_workers,
		char*** splitted_subdirs, unsigned* n_worker_subdirs)
{
	for (unsigned c_worker = 0; c_worker < n_workers; ++c_worker) {
		int fd_w = workers[c_worker]->fd_w;
		unsigned n_subdirs = n_worker_subdirs[c_worker];
		write_size(fd_w, n_subdirs);

		char** subdirs = splitted_subdirs[c_worker];
		for (unsigned c_subdir = 0; c_subdir < n_subdirs; ++c_subdir)
			write_data(fd_w, subdirs[c_subdir]);
	}
}
