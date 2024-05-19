#ifndef PATH_H
#define PATH_H

#include <stdbool.h>

void updatepwd();
void appendpath(const char *dir);
int cd(char **argv);
void forEachPath(void (*action)(const char *p));
void dnormilize_path(char **path);
bool isFileExists(const char *file);
bool isFileExistsPath(const char *file);
void prettify_path(char **path);
void unprettify_path(char **path);

#endif
