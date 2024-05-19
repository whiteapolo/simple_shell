#include "trie.h"
#include "lll.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "string_builder.h"

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
	while (*(str)) {
		if ((cnt = TrieGetiChild(cnt, *str)) == NULL)
			return false;
		str++;
	}
	return true;
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

void TrieCollectInfos(Trie *t, StringBuilder *sb)
{
	if (t == NULL)
		return;

	if (t->str != NULL)
		StringBuilderAppend(sb, t->str);

	lll_t *cnt = t->children;
	while (cnt != NULL) {
		TrieCollectInfos(cnt->info, sb);
		cnt = cnt->next;
	}
}

char **TrieGetAllMatches(Trie *t, const char *str, int *size)
{
	*size = 0;
	Trie *cnt = t;
	while (*(str)) {
		if ((cnt = TrieGetiChild(cnt, *str)) == NULL)
			return NULL;
		str++;
	}

	StringBuilder *sb = StringBuilderCreate();
	TrieCollectInfos(cnt, sb);
	*size = sb->size;

	return StringBuilderToStrings(sb, true);
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
