#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "data_structers/lll.h"
#include "io.h"
#include "lexer.h"
#include "shell.h"
#include "alias.h"

#define IS_NOT_EMPTY_STRING(str) (str && *str)

static char *home_path;


char *get_home_path()
{
	return strcat(strdup(getpwuid(geteuid())->pw_dir), "/");
}

void main(void)
{
	char *cmd;
	lll_t *tokens;
	char *home_path = get_home_path();

	shell_init(home_path);
	alias_init();
	io_init(home_path);

	while (!shell_should_exit()) {
		/* shell_update_files_completion_tree(&files); */
		cmd = io_input_command();
		if (IS_NOT_EMPTY_STRING(cmd)) {
			tokens = tokinize(cmd);
			alias_proccess(&tokens);
			shell_excute(tokens, env);
			lll_clear(&tokens, free);
		}
		free(cmd);
	}
}
