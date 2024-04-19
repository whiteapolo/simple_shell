#include "shell.h"
#include "data_structers/lll.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "string_builder.h"

int get_program_path(const char *exe, char *pw);
bool excute_in_dict(char **argv);
void cd(char **argv);
void exit_ex(char **argv);

void argv_free(char **argv);

typedef struct {
	char *name;
	void (*fun)(char**);
} Program;

Program programs[] = {
	{"cd", cd},
	{"exit", exit_ex},
};

static Dict programs_dict;
static char *home_path;
static bool should_exit = false;

#define IS_FILE_EXISTS(p) (!access(p, F_OK))


void shell_init(char *home)
{
	home_path = home;

	dict_init(&programs_dict, ARRAY_SIZE(programs));

	FOR(i, ARRAY_SIZE(programs))
		dict_insert(&programs_dict, programs[i].name, programs[i].fun);

	/* completion_tree_init(programs_tree); */
	/* *files_tree = NULL; */
	/* shell_update_files_completion_tree(files_tree); */
}

/* void shell_update_files_completion_tree(Completion_tree **files) */
/* { */
/* 	completion_tree_destory(files); */
/* 	completion_tree_init(files); */
/* 	/1* add programs to tree *1/ */
/* } */

int get_program_path(const char *exe, char *pw)
{
	FOR (i, ARRAY_SIZE(g_path)) {
		strcpy(pw, g_path[i]);
		strcat(pw, exe);
		if (IS_FILE_EXISTS(pw))
			return 0;
	}
	return 1;
}


int excute(char **argv, bool in_background)
{
	if (*argv == NULL)
		return 0;

	/* argv_print(argv); */
	int exit_status;

	char pw[256];
	if (!excute_in_dict(argv)) {
		if (!get_program_path(*argv, pw)) {
			if (fork()) {
				if (!in_background)
					wait(&exit_status);
			} else {
				execve(pw, argv, env);
			}
		} else {
			printf("command doesnt exists: %s\n", *argv);
		}
	}
	return exit_status;
}

void shell_excute(lll_t *tokens, char **env)
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

void argv_print(char **argv)
{
	while (*argv)
		printf("%s\n", *(argv++));
}

bool excute_in_dict(char **argv)
{
	void (*fun)(char**) = dict_find(&programs_dict, *argv);
	if (fun) {
		fun(argv);
		return true;
	} else {
		return false;
	}
}

bool shell_should_exit()
{
	return should_exit;
}

// Programs
void cd(char **argv)
{
	char p[256];

	if (argv[1] == NULL)
		strcpy(p, home_path);
	else if (!strncmp(*argv, "~/", 2))
		sprintf(p, "%s%s", home_path, argv[1]+2);
	else
		strcpy(p, argv[1]);

	if (IS_FILE_EXISTS(p))
		chdir(p);
	else
		printf("cd: no such file or directory: %s\n", p);
}

void exit_ex(char **argv)
{
	should_exit = true;
}
