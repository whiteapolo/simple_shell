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

void shell_init();
void shell_excute(lll_t *tokens);
bool shell_should_exit();

#endif
