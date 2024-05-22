#ifndef READLINE_H
#define READLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <pwd.h>

#include "types.h"
#include "data_structers/dict.h"
#include "data_structers/trie.h"
#include "history.h"

#define KEY_TAB 	9
#define KEY_CONTROL_L 	12
#define KEY_RETURN 	127
#define KEY_SPACE 	32
#define KEY_ARROW_UP 	128
#define KEY_ARROW_DOWN 	129
#define KEY_ARROW_RIGHT 130
#define KEY_ARROW_LEFT 	131

char *ReadLine(const char *prompt);
void ReadLineInit(HistoryEntry *hist_entry, Trie *syntax_trie);
void ReadLineDestroy();

#endif
