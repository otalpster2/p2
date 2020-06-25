#ifndef SUBDIRS_H
#define SUBDIRS_H

/*
 * Read contents of directory 'path' and return them.
 * Also checks if the contents are all directories.
 * If they are not the function returns NULL.
 *
 * path: path to root directory containing subdirectories to return.
 * n_dirs: the number of directories returned.
 *
 * Success: returns array of path strings to subdirectories.
 * Failure: returns NULL and sets 'error_code'.
 */
char** parse_subdirs(char* base_path, unsigned* n_dirs);

/*
 * Partition the strings in 'subdirs' into 'n_workers' parts.
 * Memory for the strings used by 'subdirs' is re-used by the returned array.
 *
 * 'subdirs': is the array of strings to partition.
 * 'n_subdirs': number of strings in 'subdirs'.
 * 'n_workers': how many partitions to create.
 * 'n_worker_subdirs': number of strings per partition.
 *
 * Success: returns array of 'n_workers' rows.
 * Each row contains '(*n_worker_subdirs)[row]' strings.
 * Failure: returns NULL and sets 'error_code'.
 * On system error calls 'perror()' and exits.
 */
char*** split_subdirs(char** subdirs, unsigned n_subdirs,
		unsigned n_workers, unsigned** n_worker_subdirs);

void send_subdirs(struct worker** workers, unsigned n_workers,
		char*** splitted_subdirs, unsigned* n_worker_subdirs);

#endif  // #ifndef SUBDIRS_H
