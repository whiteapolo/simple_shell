#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>

#include "path.h"
#include "data_structers/string_builder.h"
#include "types.h"

void updatepwd()
{
	char *pwd = getcwd(NULL, 1024);
	setenv("PWD", pwd, 1);
	free(pwd);
}

void appendpath(const char *dir)
{
	const char *path = getenv("PATH");
	char *value;

	if (path) {
		if (!strstr(path, dir)) {
			value = str3dup(path, ":", dir);
			setenv("PATH", value, 1);
			free(value);
		}
	} else {
		setenv("PATH", dir, 1);
	}
}

int cd(char **argv)
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

	if (isFileExists(p)) {
		chdir(p);
	} else {
		printf("cd: no such file or directory: %s\n", p);
		return 1;
	}
	updatepwd();
	return 0;
}

void forEachPath(void (*action)(const char *p))
{
	const char *path = getenv("PATH");
	if (path == NULL)
		return;

	char *pdup = strdup(path);
	char *p = strtok(pdup, ":");
	while (p != NULL) {
		action(p);
		p = strtok(NULL, ":");
	}

	free(pdup);
}

// replace /home/USER with ~
void prettify_path(char **path)
{
	const char *home = getenv("HOME");

	if (home == NULL)
		return;

	char *home_dup = str2dup(home, "/");
	strcut(path, home_dup, "~/");
	if (strstr(*path, home))
		str_realloc_cpy(path, "~");
}

void unprettify_path(char **path)
{
	const char *home = getenv("HOME");

	if (home == NULL)
		return;

	strcut(path, "~", home);
}


bool isFileExists(const char *file)
{
	return !access(file, F_OK);
}

bool isFileExistsPath(const char *file)
{
	const char *path = getenv("PATH");
	if (path == NULL)
		return false;

	char *saved_ptr;
	char *path_dup = strdup(path);
	char *tok = strtok_r(path_dup, ":", &saved_ptr);
	bool exists = false;

	while (!exists && tok != NULL) {
		const char *strings[] = { tok, "/", file };
		char *name = strconcat(strings, ARRAY_SIZE(strings));
		/* char *name = str3dup(tok, "/", file); */
		if (isFileExists(name))
			exists = true;
		tok = strtok_r(NULL, ":", &saved_ptr);
		free(name);
	}
	free(path_dup);
	return exists;
}
