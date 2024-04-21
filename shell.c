#include "shell.h"
#include "data_structers/lll.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "string_builder.h"
#include <wh/macros.h>

int p_cd(char **argv);
int p_exit(char **argv);

void argv_free(char **argv);
void argv_print(char **argv);

typedef struct {
	char *name;
	int (*fun)(char**);
} Program;

Program programs[] = {
	{"cd", p_cd},
	{"exit", p_exit},
};

static Dict programs_dict;
static bool should_exit = false;

#define FILE_EXISTS(p) (!access(p, F_OK))

void shell_init()
{
	dict_init(&programs_dict, ARRAY_SIZE(programs));

	FOR(i, ARRAY_SIZE(programs))
		dict_insert(&programs_dict, programs[i].name, programs[i].fun);
}

int excute(char **argv, bool in_background)
{
	int exit_status;
	int (*fun)(char**) = NULL;

	if (*argv == NULL)
		return 0;

	if ((fun = dict_find(&programs_dict, *argv))) {
		exit_status = fun(argv);
	} else {
		if (fork()) {
			if (!in_background)
				wait(&exit_status);
		} else {
			execvp(argv[0], argv);
			printf("command doesnt exists: %s\n", *argv);
			exit(1);
		}
	}
	return exit_status;
}

void shell_excute(lll_t *tokens)
{
	lll_t *cnt = tokens;
	char **argv;

	StringBuilder *sb;
	StringBuilderInit(&sb);

	while (cnt) {
		if (get_token(cnt)->type != TYPE_COMMAND) {
			argv = StringBuilderToStrings(sb, true);
			StringBuilderInit(&sb);
			switch (get_token(cnt)->type) {
			case TYPE_SEMICOLON:
				excute(argv, false);
				break;
			case TYPE_AMPERCENT:
				excute(argv, true);
				break;
			case TYPE_DOUBLE_AMPERCENT:
				/* if (excute(argv, false)) { */
				/* 	argv_free(argv); */
				/* 	return; */
				/* } */
				break;
			case TYPE_PIPE:
				/* excute(argv, false); */	
				break;
			default:
				excute(argv, false);
				break;
			argv_free(argv);
			}
		} else {
			StringBuilderAppend(sb, get_token(cnt)->info);
		}
		cnt = cnt->next;
	}

	argv = StringBuilderToStrings(sb, true);
	excute(argv, false);
}

void argv_free(char **argv)
{
	int i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

bool shell_should_exit()
{
	return should_exit;
}

// Programs
int p_cd(char **argv)
{
	char p[256];
	const char *home_path = getenv("HOME");

	if (home_path == NULL)
		home_path = ".";

	if (argv[1] == NULL)
		strcpy(p, home_path);
	else if (!strncmp(*argv, "~/", 2))
		sprintf(p, "%s%s", home_path, argv[1]+2);
	else
		strcpy(p, argv[1]);

	if (FILE_EXISTS(p)) {
		chdir(p);
	} else {
		printf("cd: no such file or directory: %s\n", p);
		return 1;
	}
	return 0;
}

int p_exit(char **argv)
{
	should_exit = true;
	return 0;
}

// debigging
void argv_print(char **argv)
{
	while (*argv)
		printf("%s\n", *(argv++));
}
