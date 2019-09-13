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

#include <sys/types.h>
#include <sys/ioctl.h>

#include <fcntl.h>

#include <unistd.h>
#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <bits/ioctl.h>

#define BUFSIZE 1024

char buf[BUFSIZE];
struct winsize wsz;

int main(int argc, char **argv) {
	int quit = 0;
	int fd = STDIN_FILENO, nb_bytes, line_max, columns_max;
	int cpt_columns = -1, cpt_line = 0;
	int err;
	char key;

	if (argc > 2) {
		printf("Usage: more <file>\n");
		return 1;
	}

	if (argc == 2) {
		fd = open(argv[1], O_RDONLY);
		if (fd == -1) {
			printf("Error #%d:\n  Unable to open %s\n", errno, argv[1]);
			return 2;
		}
	}
	/* Get number of lines and columns */
	err = ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsz);
	if (err != 0) {
		printf("Errno: %d\n  ioctl error %d\n", errno, err);
		return 3;
	}

	line_max = wsz.ws_row;
	columns_max = wsz.ws_col;

	nb_bytes = BUFSIZE;

	while ((nb_bytes = read(fd, buf, BUFSIZE)) > 0) {

		int i;

		for (i = 0; i < nb_bytes; i++) {
			cpt_columns++;

			if ((int) buf[i] == 9) { /* Horizontal TAB */
				cpt_columns += 7 - (cpt_columns % 8);
				if (cpt_columns >= columns_max) {
					cpt_columns = 8;
					cpt_line++;
					putchar(13);
					putchar(10);
				  }
			} else {
				if (cpt_columns == columns_max) { /* End of line */
					cpt_columns = -1;
					cpt_line++;
					putchar(13);
					putchar(10);
				} else if ((int) buf[i] == 10) { /* Line feed in text */
					cpt_columns = -1;
					cpt_line++;

				}

				if (cpt_line == (line_max - 2)) {
					if ((int) buf[i] == 10) {
						putchar(buf[i++]); /* print Line feed before ---MORE--- */
					}

					cpt_line = 0;

					printf("\n--MORE--"); fflush(stdout);

					key = getc(stderr);
					if ((key == 'q') || (key == 'Q')) {
						quit = 1;
						break;
					}

					putchar('\n');
				}

				putchar(buf[i]);
			}
		}

		if (quit) {
			break;
		}

	}

	putchar('\n');

	return 0;
}


