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
#include <stdio.h>
#include <string.h>
#include <syscall.h>
#include <libc.h>
#include <hidestr.h>
#include <execv_safe.h>

#define TOKEN_NR	10
#define TOKEN_LENGTH	80
#define TOKEN_DELIM	" "
#define ARGS_MAX	16

char tokens[TOKEN_NR][TOKEN_LENGTH];

void hide_str(const char* input_str)
{
	if (input_str == NULL)
		return;

	size_t str_size= strlen(input_str) + 1;
	char* str = malloc(str_size);
	strncpy(str, input_str, str_size);

	hidestr(str, str_size);

	printf("%s\n", str);
	free(str);
}

int check_token(char *str) {
	if (strlen(str) > TOKEN_LENGTH) {
		printf("Erreur: Token plus long que (%d)\n", TOKEN_LENGTH);
		return 1;
	}

	return 0;
}

void parse_token(char *str) {
	int i = 0;
	char *next_token;

	next_token = strtok(str, TOKEN_DELIM);
	if (check_token(next_token) || !next_token)
		return;

	strncpy(tokens[i++], next_token, TOKEN_LENGTH);

	while ((next_token = strtok(NULL, TOKEN_DELIM)) != NULL) {
		if ((i < TOKEN_NR) && !check_token(next_token)) {
			strncpy(tokens[i++], next_token, TOKEN_LENGTH);
		} else {
			if (!(i < TOKEN_NR)) {
				printf("Erreur: nombre maximum de tokens possible (%d)\n", TOKEN_NR);
			}

			/* Invalidate tokens */
			tokens[0][0] = '\0';
			return;
		}
	}
}

/*
 * Process the command with the different tokens
 */
void process_cmd(void) {
	int arg_pos;
	char *argv[ARGS_MAX];
	char filename[30];

	if (!strcmp(tokens[0], "hide_str")) {
		hide_str(tokens[1]);
		return;
	}

	/* Prepare the arguments to be passed to exec() syscall */
	arg_pos = 0;

	while (tokens[arg_pos][0] != 0) {
		argv[arg_pos] = tokens[arg_pos];
		arg_pos++;
	}

	/* Terminate the list of args properly */
	argv[arg_pos] = NULL;

	strcpy(filename, argv[0]);
	strcat(filename, ".elf");

	if (execv(filename, argv) == -1) {
		printf("%s: execv failed.\n", argv[0]);
	}
}


/*
 * Main entry point of the shell application.
 */
void main(int argc, char *argv[])
{
	char prompt[] = "so3% ";
	char user_input[80];

	while (1) {
		printf("%s", prompt);
		fflush(stdout);

		gets(user_input);

		if (strcmp(user_input, ""))
			parse_token(user_input);

		/* Check if there is at least one token to be processed */
		if (tokens[0][0] != 0)
			process_cmd();

	}
}
