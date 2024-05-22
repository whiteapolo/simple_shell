#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <dirent.h>
#include <wh/messure_time.h>

#include "completion.h"
#include "data_structers/dict.h"
#include "data_structers/mystrings.h"
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

void initenv()
{
	unsetenv("SHELL");
	setenv("SHELL", "/home/white/.local/bin/mysh", 1);
	setenv("HOME", getpwuid(geteuid())->pw_dir, 1);

	unsetenv("PATH");
	FOR(i, ARRAY_SIZE(path)) {
		appendpath(path[i]);
	}
	updatepwd();
}

char *getpromt()
{
	char *cwd = get_prettify_cwd();

	if (cwd == NULL)
		return strdup(promt);

	char *result = str4dup("\e[0;35m", cwd, "\e[0m", promt);
	free(cwd);

	return result;
}

char *inputcommand()
{
	char *promt = getpromt();
	char *command = ReadLine(promt);
	free(promt);
	return command;
}

void init_alias_dict(Dict *alias_dict)
{
	dict_init(alias_dict, ARRAY_SIZE(aliases));
	FOR(i, ARRAY_SIZE(aliases)) {
		dict_insert(alias_dict, aliases[i][0], aliases[i][1]);
	}
}

void init_programs_dict(Dict *programs_dict)
{
	dict_init(programs_dict, 2);
	dict_insert(programs_dict, "cd", cd);
	dict_insert(programs_dict, "exit", exit);
}

void init_syntax_trie(Trie *t)
{
	TrieInit(t);
	TrieAdd(t, "exit");
	TrieAdd(t, "cd");

	FOR(i, ARRAY_SIZE(aliases)) {
		TrieAdd(t, aliases[i][0]);
	}

	FOR_EACH_PATH(path,
		FOR_EACH_FILE_IN_DIR(path, file, 
			TrieAdd(t, file);
		);
	);
}

int main(void)
{
	HistoryEntry hist_entry;
	char *command;
	Dict alias_dict;
	Dict programs_dict;
	Trie syntax_trie;
	Cmd *cmd;

	initenv();
	HistoryInit(&hist_entry, history_file);
	init_alias_dict(&alias_dict);
	init_programs_dict(&programs_dict);
	init_syntax_trie(&syntax_trie);

	ReadLineInit(&hist_entry, &syntax_trie);

	while (1) {
		command = inputcommand(&syntax_trie);
		if (!IS_EMPTY_COMMAND(command)) {
			cmd = CmdCreateFromString(command);
			HistoryAppend(&hist_entry, command);
			HistorySave(&hist_entry);
			CmdRun(cmd, &programs_dict, &alias_dict, true);
		}
		free(command);
	}
}
