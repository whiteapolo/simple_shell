#ifndef COMPLETION_H
#define COMPLETION_H

#include "data_structers/trie.h"

char **complete_line(const char *line, int *matches, Trie *syntax_trie);
void show_completions(char **completions, int size);
void clear_completions();

#endif
