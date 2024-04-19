#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <wh/macros.h>
#include "lll.h"

typedef struct Trie {
	lll_t *children;
	void *info;
	char letter;
} Trie;

Trie *TrieCreateNode(char letter, void *info);
void TrieInit(Trie **t);
Trie *TrieGetiChild(Trie *t, char letter);
void TrieAdd(Trie *t, char *str, void *info);
void *TrieAddiChild(Trie *t, char letter, void *info);
void TrieDestroy(Trie *t, void (*free_info)(void *));
void *TrieGet(Trie *t, char *str);

#endif
