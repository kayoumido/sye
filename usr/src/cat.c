/*
 * Copyright (C) 2014-2017 Daniel Rossier <daniel.rossier@heig-vd.ch>
 * Copyright (C) 2017-2018 Xavier Ruppen <xavier.ruppen@heig-vd.ch>
 * Copyright (C) 2017 Alexandre Malki <alexandre.malki@heig-vd.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <unistd.h>
#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFSIZE 1024

char buf[BUFSIZE];

int main(int argc, char **argv) {
	int fd, amount;

	if (argc != 2) {
		printf("Usage: cat <file>\n");
		return 1;
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		printf("Unable to open %s\n", argv[1]);
		return 1;
	}

	while ((amount = read(fd, buf, BUFSIZE)) > 0) {
		write(1, buf, amount);
	}

	close(fd);

	return 0;
}
