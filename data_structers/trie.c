#include "trie.h"
#include "lll.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mystrings.h"
#include "../types.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void TrieInit(Trie *t)
{
	lll_init(&t->children);
	t->str = NULL;
}

Trie *TrieCreateNode(char letter, void *info)
{
	Trie *t = MALLOC(Trie, 1);
	lll_init(&t->children);
	t->letter = letter;
	t->str = info;
	return t;
}

Trie *TrieGetiChild(Trie *t, char letter)
{
	lll_t *cnt = t->children;
	while (cnt) {
		if (((Trie*)cnt->info)->letter == letter)
			return cnt->info;
		cnt = cnt->next;
	}
	return NULL;
}

bool TrieExists(Trie *t, const char *str)
{
	Trie *cnt = t;
	while (*str) {
		if ((cnt = TrieGetiChild(cnt, *str)) == NULL)
			return false;
		str++;
	}
	return cnt->str;
}

Trie *TrieGet(Trie *t, const char *str)
{
	while (*str) {
		if ((t = TrieGetiChild(t, *str)) == NULL)
			return NULL;
		str++;
	}
	return t;
}

void *TrieAddiChild(Trie *t, char letter, void *info)
{
	lll_push(&t->children, TrieCreateNode(letter, info));
	return t->children->info;
}

void TrieAdd(Trie *t, const char *str)
{
	Trie *cnt = t;
	Trie *father = cnt;
	const char *str_start = str;

	while (*str) {
		if ((cnt = TrieGetiChild(cnt, *str)) == NULL) {
			cnt = TrieAddiChild(father, *str, NULL);
		}
		father = cnt;
		str++;
	}

	cnt->letter = *(str-1);

	cnt->str = strdup(str_start);
}

void TrieCollectInfos(Trie *t, Sbuilder *sb)
{
	if (t == NULL)
		return;

	if (t->str != NULL) {
		SbuilderAppend(sb, t->str);
	}

	lll_t *cnt = t->children;
	while (cnt != NULL) {
		TrieCollectInfos(cnt->info, sb);
		cnt = cnt->next;
	}
}


char **TrieGetAllMatches(Trie *t, const char *str, int *size)
{
	int i = 0;
	if (size != NULL) *size = 0;

	t = TrieGet(t, str);
	if (t == NULL) {
		return EMPTY_SPLIT();
	}

	Sbuilder *sb = SbuilderCreate();
	SbuilderAppend(sb, "");
	TrieCollectInfos(t, sb);
	int len = sb->size;
	if (size != NULL) *size = len;

	char **completions = SbuilderToStrings(sb, true);
	free(*completions);
	char *prefix = common_prefix(completions + 1, len - 1);
	*completions = strdup(prefix + strlen(str));
	free(prefix);

	return completions;
}

void _TrieDestroy(void *t)
{
	if (((Trie*)t)->children != NULL)
		lll_clear(&((Trie*)t)->children, _TrieDestroy);
	free(((Trie*)t)->str);
	free(t);
}

void TrieDestroy(Trie *t)
{
	if (t->children != NULL)
		lll_clear(&t->children, _TrieDestroy);
}


bool TrieIsLeaf(Trie *t)
{
	return (*t).children == NULL;
}
