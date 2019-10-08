/*
 * Copyright (C) 2019 Kevin Joly <kevin.joly@heig-vd.ch>
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
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <libc.h>

/*
 * Main entry point of the shell application.
 */
void main(int argc, char *argv[])
{
	printf("             . . .\n");
	printf("              \\|/\n");
	printf("            `--+--'\n");
	printf("              /|\\\n");
	printf("             ' | '\n");
	printf("               |\n");
	printf("               |\n");
	printf("           ,--'#`--.\n");
	printf("           |#######|\n");
	printf("        _.-'#######`-.\n");
	printf("     ,-'###############`-.\n");
	printf("   ,'#####################`,\n");
	printf("  /#########################\\\n");
	printf(" |###########################|\n");
	printf("|#############################|\n");
	printf("|#############################|\n");
	printf("|#############################|\n");
	printf("|#############################|\n");
	printf(" |###########################|\n");
	printf("  \\#########################/\n");
	printf("   `.#####################,' \n");
	printf("     `._###############_,' \n");
	printf("        `--..#####..--'\n\n");
	printf("You got pwned!");
	fflush(stdout);


	while (1) {
		sleep(1);
	}
}
