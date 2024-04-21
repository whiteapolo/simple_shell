#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data_structers/lll.h"
#include "io.h"
#include "lexer.h"
#include "shell.h"
#include "alias.h"

#define IS_NOT_EMPTY_STRING(str) (str && *str)

static char *home_path;
static char *path;
static char path_len = 1;

void append_path(const char *p)
{
	path_len += strlen(p) + 2;
	REALLOC(path, char, path_len);
	strcat(path, ":");
	strcat(path, p);
	setenv("PATH", path, 1);
}

void env_init()
{
	setenv("HOME", strdup(getpwuid(geteuid())->pw_dir), 1);
	append_path("/bin");
	append_path("/usr/bin");
	append_path("/usr/local/bin");
	append_path("/home/white/.local/bin");
	append_path("/home/white/archive/.scripts");
}

int main(void)
{
	char *cmd;
	lll_t *tokens;
	path = CALLOC(char, path_len);

	env_init();
	shell_init();
	alias_init();
	io_init();

	while (!shell_should_exit()) {
		cmd = io_input_command();
		if (IS_NOT_EMPTY_STRING(cmd)) {
			tokens = tokinize(cmd);
			alias_proccess(&tokens);
			shell_excute(tokens);
			lll_clear(&tokens, free);
		}
		free(cmd);
	}
}
