#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include "readline/readline.h"
#include "string_builder.h"
#include "strings.h"
#include <wh/macros.h>
#include <pwd.h>
#include <unistd.h>
#include "config.h"
#include "cursors.h"
#include "readline.h"
#include <string.h>

static char *home_path;

void io_init(char *home);
char *io_input_command();
char *get_promt();

void io_init(char *home)
{
	CURSOR_TO_BLOCK();
	home_path = home;
	ReadLineInit(NULL);
}

char *io_input_command()
{
	char *promt = get_promt();
	char *cmd = ReadLine(promt);
	free(promt);
	return cmd;
}

char *get_promt()
{
	char *p = MALLOC(char, 256);
	char *cwd = getcwd(NULL, 256);
	char *home_cpy = strndup(home_path, strlen(home_path) - 1);
	char *normalize_path = strcut(cwd, home_cpy, "~");

	if (normalize_path) {
		SWAP(normalize_path, cwd);
		free(normalize_path);
	}

	strcat(strcpy(p, cwd), promt);

	free(home_cpy);
	free(cwd);

	return p;
}

#endif
