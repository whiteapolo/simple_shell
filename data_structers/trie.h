#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wh/macros.h>
#include "lll.h"

typedef struct Trie {
	lll_t *children;
	char *str;
	char letter;
} Trie;

void TrieInit(Trie *t);
char **TrieGetAllMatches(Trie *t, const char *str, int *size);
Trie *TrieCreateNode(char letter, void *info);
Trie *TrieGetiChild(Trie *t, char letter);
void TrieAdd(Trie *t, const char *str);
void *TrieAddiChild(Trie *t, char letter, void *info);
void TrieDestroy(Trie *t);
bool TrieExists(Trie *t, const char *str);

#endif
