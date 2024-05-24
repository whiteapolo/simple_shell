#include "cmd.h"
#include "data_structers/mystrings.h"
#include "data_structers/dict.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "types.h"
#include "config.h"
#include <stdio.h>

// local programs
u8 cd(char **argv);
void proccessAlias(Cmd **cmd, Dict *alias_dict);

Cmd *CmdCreateFromString(const char *str)
{
	return SbuilderCreateFromString(str, " ");
}

int CmdRun(Cmd *cmd, Dict *programs_dict, Dict *alias_dict, bool destroy)
{
	proccessAlias(&cmd, alias_dict);
	char **argv = SbuilderToStrings(cmd, destroy);
	int exit_status = 0;
	u8 (*fun)(char**) = NULL;

	if (*argv == NULL)
		return 0;

	if ((fun = dict_find(programs_dict, *argv))) {
		exit_status = fun(argv);
	} else {
		if (fork()) {
			wait(&exit_status);
		} else {
			execvp(argv[0], argv);
			printf("command doesnt exists: %s\n", *argv);
			exit(1);
		}
	}
	splitfree(argv);
	return exit_status;
}

Cmd *CmdCreate()
{
	return SbuilderCreate();
}

void CmdCat(Cmd *dest, Cmd *src)
{
	SbuilderConcat(dest, src);
}

void CmdAppend(Cmd *cmd, const char *arg)
{
	SbuilderAppend(cmd, arg);
}

void CmdDestroy(Cmd *cmd)
{
	SbuilderDestroy(cmd);
}

const char *CmdGet(Cmd *cmd, u16 n)
{
	return SbuilderGet(cmd, n);
}

void CmdRemove(Cmd *cmd, u16 n)
{
	SbuilderRemoveIndex(cmd, n);
}

void proccessAlias(Cmd **cmd, Dict *alias_dict)
{
	if (!dict_find(alias_dict, CmdGet(*cmd, 0)))
		return;

	Cmd *alias = CmdCreateFromString(dict_find(alias_dict, CmdGet(*cmd, 0)));
	CmdRemove(*cmd, 0);
	Cmd *new = CmdCreate();
	CmdCat(new, alias);
	CmdCat(new, *cmd);
	CmdDestroy(*cmd);
	CmdDestroy(alias);
	*cmd = new;
}
