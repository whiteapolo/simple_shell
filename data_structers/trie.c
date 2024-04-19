#include "trie.h"
#include <stdbool.h>

void TrieInit(Trie **t)
{
	*t = TrieCreateNode(0, NULL);
}

Trie *TrieCreateNode(char letter, void *info)
{
	Trie *t = MALLOC(Trie, 1);
	lll_init(&t->children);
	t->letter = letter;
	t->info = info;
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

/* void print_info(void *info) */
/* { */
/* 	printf("INFO: %c \n", ((Trie*)info)->letter); */
/* } */

void *TrieGet(Trie *t, char *str)
{
	Trie *cnt = t;
	while (*(str)) {
		/* lll_print(cnt->children, print_info); */
		if ((cnt = TrieGetiChild(cnt, *str)) == NULL)
			return NULL;
		str++;
	}
	return cnt->info;
}

void *TrieAddiChild(Trie *t, char letter, void *info)
{
	lll_push(&t->children, TrieCreateNode(letter, info));
	return t->children->info;
}

void TrieAdd(Trie *t, char *str, void *info)
{
	Trie *cnt = t;
	Trie *tmp = cnt;
	while (*(str)) {
		if ((cnt = TrieGetiChild(cnt, *str)) == NULL) {
			cnt = TrieAddiChild(tmp, *str, NULL);
		}
		tmp = cnt;
		str++;
	}
	cnt->letter = *(str-1);
	cnt->info = info;
}

/* free info can be null */
void TrieDestroy(Trie *t, void (*free_info)(void *))
{
	/* print_info(t); */
	lll_t *cnt = t->children;
	while (cnt) {
		TrieDestroy(cnt->info, free_info);
		cnt = cnt->next;
	}
	if (t->children != NULL) {
		lll_clear(&t->children, NULL);
	}
	if (free_info != NULL)
		free_info(t->info);
	free(t);
	t = NULL;
}

bool TrieIsLeaf(Trie *t)
{
	return (*t).children == NULL;
}
