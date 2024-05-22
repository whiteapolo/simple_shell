#ifndef PATH_H
#define PATH_H

#include <stdbool.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

#define FOR_EACH_PATH(varname, code) 		\
do { 						\
	const char *_path = getenv("PATH"); 	\
	if (_path == NULL) 			\
		break; 				\
	char *pdup = strdup(_path); 		\
	char *varname = strtok(pdup, ":"); 	\
	while (varname != NULL) { 		\
		do { code } while (0); 		\
		varname = strtok(NULL, ":"); 	\
	} 					\
	free(pdup); 				\
} while (0)

#define FOR_EACH_FILE_IN_DIR(dir, varname, code) 	\
do { 							\
	struct dirent *_de; 				\
	DIR *_dr = opendir(dir); 			\
	if (_dr == NULL) 				\
		break; 					\
	while ((_de = readdir(_dr)) != NULL) { 		\
		const char *varname = _de->d_name; 	\
		do { code } while (0); 			\
	} 						\
	closedir(_dr);  				\
} while (0)

bool is_dir(const char *path);
bool is_file(const char *path);

void updatepwd();
void appendpath(const char *dir);
int cd(char **argv);
void forEachPath(void (*action)(const char *p));
void dnormilize_path(char **path);
bool isFileExists(const char *file);
bool isFileExistsPath(const char *file);
char **path_complete(const char *subpath);

void prettify_path(char **path);
void unprettify_path(char **path);

char *get_prettify_cwd();

#endif
