/*
 *
 * ----- SO3 Smart Object Oriented (SOO) Operating System -----
 *
 * Copyright (c) 2016,2017 Sootech SA, Switzerland
 * Copyright (c) 2014-2018 REDS Institute, HEIG-VD, Switzerland
 *
 * This software is released under the MIT License whose terms are defined hereafter.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*
 * Main function of ls application.
 * The ls application is very very short and does not support any options like -l -a etc.
 * It is only possible to give a subdir name to list the directory entries of this subdir.
 */
int main(int argc, char **argv) {
	DIR *stream;
	struct dirent  *p_entry;
	char *dir;

	if (argc == 1)
		dir = ".";
	else if (argc == 2)
		dir = argv[1];
	else {
		printf("Usage: ls [DIR]\n");
		exit(1);
	}

	stream = opendir(dir);

	if (stream == NULL)
		exit(1);

	while ((p_entry = readdir(stream)) != NULL)
		printf("%s\n", p_entry->d_name);

	exit(0);
}

