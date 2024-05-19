#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include "data_structers/dict.h"
#include "data_structers/string_builder.h"
#include "data_structers/lll.h"
#include "cmd.h"
#include "config.h"
#include "data_structers/trie.h"
#include "readline.h"
#include "mycursors.h"
#include "line.h"
#include "types.h"
#include "history.h"
#include "path.h"

void envinit()
{
	setenv("HOME", getpwuid(geteuid())->pw_dir, 1);

	unsetenv("PATH");
	FOR(i, ARRAY_SIZE(path)) {
		appendpath(path[i]);
	}
	updatepwd();
}

char *getpromt()
{
	char *cwd = getcwd(NULL, 256);
	const char *home = getenv("HOME");

	if (cwd == NULL)
		return strdup(promt);

	prettify_path(&cwd);
	char *result = str4dup("\e[0;35m", cwd, "\e[0m", promt);

	free(cwd);
	return result;
}

char *inputcommand(Dict *highlight_dict)
{
	char *promt = getpromt();
	char *command = ReadLine(promt, highlight_dict);
	free(promt);
	return command;
}

void alias_init(Dict *alias_dict)
{
	dict_init(alias_dict, ARRAY_SIZE(aliases));
	FOR(i, ARRAY_SIZE(aliases)) {
		dict_insert(alias_dict, aliases[i][0], aliases[i][1]);
	}
}

void programs_init(Dict *programs_dict)
{
	dict_init(programs_dict, 2);
	dict_insert(programs_dict, "cd", cd);
	dict_insert(programs_dict, "exit", exit);
}

void highlight_init(Dict *highlight_dict)
{
	dict_init(highlight_dict, 2 + ARRAY_SIZE(aliases));
	FOR(i, ARRAY_SIZE(aliases)) {
		dict_insert(highlight_dict, aliases[i][0], (void*)1);
	}
	dict_insert(highlight_dict, "cd", (void*)1);
	dict_insert(highlight_dict, "exit", (void*)1);
}

void at_exit()
{
	/* putchar('\n'); */
	HistorySave();
	fflush(stdout);
	exit(0);
}

void print(const char *p)
{
	printf("%s\n", p);
}

void init_syntax_trie(Trie *t)
{
	TrieAdd(t, "echo");
	TrieAdd(t, "exit");
	TrieAdd(t, "cneofetch");

	/* forEachPath(print); */

}

int main(void)
{
	char *command;
	Dict alias_dict;
	Dict programs_dict;
	Dict highlight_dict;
	Trie syntax_trie;
	Cmd *cmd;

	envinit();
	HistoryInit();
	signal(SIGTERM, at_exit);
	signal(SIGINT, at_exit);
	signal(SIGKILL, at_exit);
	atexit(at_exit);
	alias_init(&alias_dict);
	programs_init(&programs_dict);
	highlight_init(&highlight_dict);
	CURSOR_TO_BLOCK();
	ReadLineInit(NULL);
	init_syntax_trie(&syntax_trie);
	ReadLineSetSyntaxTrie(&syntax_trie);

	while (1) {
		command = inputcommand(&highlight_dict);
		if (!IS_EMPTY_STRING(command)) {
			cmd = CmdCreateFromString(command);
			HistoryAppend(command);
			CmdRun(cmd, &programs_dict, &alias_dict, true);
		}
		free(command);
	}
}
