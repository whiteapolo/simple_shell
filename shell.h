#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <wh/macros.h>
#include "data_structers/dict.h"
#include "config.h"
#include "lexer.h"

void shell_init(char *home);
void shell_excute(lll_t *tokens, char **env);
bool shell_should_exit();
/* void shell_update_files_completion_tree(Completion_tree **files); */

#endif
