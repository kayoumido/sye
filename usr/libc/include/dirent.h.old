/*
 * dirent.h
 *
 *  Created on: 20 août 2008
 *      Author: drossier
 */

#ifndef DIRENT_H_
#define DIRENT_H_

#include "stddef.h"
#include "syscall.h"

/* Type of file similar as linux*/
#define DT_UNKNOWN	0 /* Unknown */
#define DT_FIFO		1 /* Fifo file */
#define DT_CHR		2 /* Charactere device file */
#define DT_DIR		4 /* Directory file */
#define DT_BLK		6 /* Block device file */
#define DT_REG		8 /* Regular file */


struct dirent {
	char d_name[256];
	int d_type; /* true if != 0 */
	int d_size;
};

typedef void *DIR;

/**
 * The opendir() function opens a directory stream corresponding to the
 * directory name, and returns a pointer to the directory  stream. The
 * stream is positioned at the first entry in the directory.
 *
 * Returns a pointer to the DIR structure, or NULL on error and set errno.
 */
DIR *opendir(const char *name);

/**
 * The readdir() function returns a pointer to a dirent structure representing
 * the next directory entry in the directory stream.
 *
 * Returns NULL on reaching the end of the directory stream or if an error
 * occurred.
 */
struct dirent *readdir(DIR *dir);

/**
 * The chdir() function shall cause the directory named  by  the  pathname
 * pointed  to  by  the path argument to become the current working directory;
 * that is, the starting point for path searches for  pathnames  not beginning
 * with '/' .
 *
 * Returns 0 on success or -1 on error and set errno.
 */
int chdir(const char *name);

/**
 * This function return a null-terminated string containing an absolute
 * pathname  that is the current working directory of the calling process.
 * The pathname is returned as the function result and via the argument
 * buf, if present.
 *
 * The getcwd() function copies an absolute pathname of the current
 * working directory to the array pointed to by buf, which is of length size.
 *
 * If the length of the absolute pathname of the current  working  directory,
 * including the terminating null byte, exceeds size bytes, NULL is returned,
 * and errno is set to ERANGE; an application should  check  for this error,
 * and allocate a larger buffer if necessary.
 *
 * Return a pointer to a string containing the pathname of the current
 * working directory on success. This is the same value as buf.
 */
char *getcwd(char *path, size_t size);

/**
 * mkdir() attempts to create a directory named pathname.
 *
 * Returns zero on success, or -1 if an error occurred (in which case, errno
 * is set appropriately).
 */
int mkdir(const char *pathname, mode_t mode);

/**
 * The closedir() function shall close the stream opened by opendir().
 * The DIR * must be provided to to so
 */
void closedir(DIR *dir);


/**
 * These functions return information about a file. No permissions are required on the file itself,
 * but -- in the case of stat() and lstat() -- execute (search) permission is required on all of the
 * directories in path that lead to the file.
 */
#define FILENAME_SIZE 256
#define FILEDATE_SIZE 32

struct stat {
	char	st_name[FILENAME_SIZE]; 		/* Filename */
	unsigned long	st_size; 				/* Size of file */
	unsigned char	st_time[FILEDATE_SIZE]; /* Last modification date */
	unsigned char	st_flags;				/* Regular file flag*/
	unsigned char	st_mode;				/* Protection */
};

int stat(char *filename, struct stat *info);


#endif /* DIRENT_H_ */
