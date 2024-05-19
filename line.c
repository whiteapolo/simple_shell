#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "line.h"
#include "data_structers/dict.h"
#include "data_structers/string_builder.h"
#include "mycursors.h"
#include "types.h"
#include "data_structers/double_lll.h"
#include "data_structers/trie.h"
#include "path.h"

Glypn *GLYPN(void *g)
{
	return (Glypn*)g;
}

Glypn *makeGlypn(char ch, u8 color, bool bold)
{
	Glypn *glpyn = malloc(sizeof(Glypn));
	glpyn->ch = ch;
	glpyn->color = color;
	glpyn->bold = bold;
	return glpyn;
}

void LineInit(Line *line)
{
	dlist_init(&line->glpyns);
	dlist_push(&line->glpyns, NULL);
	line->cursor = line->glpyns;
}

void LineAppend(Line *line, char ch, u8 color)
{
	dlist_insert_after(line->cursor, makeGlypn(ch, color, false));
	line->cursor = line->cursor->next;
}

void LineAppendString(Line *line, const char *str, u8 color)
{
	while (*str) {
		LineAppend(line, *str, color);
		str++;
	}
}

void LineDelete(Line *line)
{
	if (line->cursor->info != NULL)
		free(dlist_remove(&line->cursor));
}

void LineClear(Line *line)
{
	while (line->cursor->next != NULL)
		LineCursorRight(line, 1);

	while (line->cursor->pre != NULL)
		LineDelete(line);
}

void LineSetString(Line *line, const char *str, u8 color)
{
	LineClear(line);
	LineAppendString(line, str, color);
}

void dlist_tok(dlist_t **l, char *dest, u16 n, char c)
{

	while (*l != NULL && GLYPN((*l)->info)->ch == c) {
		*l = (*l)->next;
	}

	int i = 0;
	while (i < n - 1 && *l != NULL && GLYPN((*l)->info)->ch != ' ') {
		dest[i] = GLYPN((*l)->info)->ch;
		*l = (*l)->next;
		i++;
	}
	dest[i] = '\0';

	while (*l != NULL && GLYPN((*l)->info)->ch == ' ') {
		*l = (*l)->next;
	}
}

void LineSetColor(Line *line, u8 color)
{
	dlist_t *cnt = line->glpyns->next;
	while (cnt != NULL) {
		GLYPN(cnt->info)->color = color;
		cnt = cnt->next;
	}
}


void HighlightWord(dlist_t *word, u8 color, bool bold)
{
	while (word && GLYPN(word->info)->ch != ' ') {
		GLYPN(word->info)->color = color;
		GLYPN(word->info)->bold = bold;
		word = word->next;
	}
}

char *wordTok(dlist_t **lst)
{
	StringBuffer *sb = StringBufferCreate();
	while (*lst && GLYPN((*lst)->info)->ch != ' ') {
		StringBufferAppend(sb, GLYPN((*lst)->info)->ch);
		*lst = (*lst)->next;
	}

	while (*lst && GLYPN((*lst)->info)->ch == ' ') {
		*lst = (*lst)->next;
	}

	char *word = StringBufferToString(sb, true);
	if (IS_EMPTY_STRING(word)) {
		free(word);
		word = NULL;
	}
	return word;
}

void LineHighlight(Line *line, Dict *highlight_dict)
{
	LineClearHighlight(line);
	dlist_t *cnt = line->glpyns->next;
	dlist_t *before = cnt;
	char *word = wordTok(&cnt);
	int word_count = 0;

	while (word != NULL) {
		if (word[0] == '-')
			HighlightWord(before, CYAN, false);
		else if (isFileExists(word))
			HighlightWord(before, MAGENTA, false);
		else if (isFileExistsPath(word))
			HighlightWord(before, GREEN, false);
		else if (dict_find(highlight_dict, word))
			HighlightWord(before, GREEN, false);
		else if (word_count == 0)
			HighlightWord(before, RED, true);
		free(word);
		before = cnt;
		word = wordTok(&cnt);
		word_count++;
	}
}

void LineClearHighlight(Line *line)
{
	LineSetColor(line, 0);
}

int getCursorRightPadding(Line *line)
{
	dlist_t *cnt = line->cursor;
	int i = 0;
	while (cnt) {
		cnt = cnt->next;
		i++;
	}
	return i - 1;
}

void LineMatchCursor(Line *line, Trie *trie)
{
	dlist_t *cnt = line->cursor;

	if (cnt->pre == NULL)
		return;
	if (GLYPN(cnt->info)->ch == ' ')
		return;
	if (cnt->pre->info != NULL && GLYPN(cnt->pre->info)->ch == ' ')
		return;

	while (cnt->pre != NULL && GLYPN(cnt->info)->ch != ' ')
		cnt = cnt->pre;

	cnt = cnt->next;

	char *substr = wordTok(&cnt);

	int size;
	char **completions = TrieGetAllMatches(trie, substr, &size);

	if (size == 1) {
		char *ptr = *completions + strlen(substr);
		LineAppendString(line, ptr, 0);
	}

	splitfree(completions);
	free(substr);
}

void LinePrint(Line *line)
{
	dlist_t *cnt = line->glpyns->next;
	while (cnt) {
		u8 bold = GLYPN(cnt->info)->bold;
		u8 color = GLYPN(cnt->info)->color;
		u8 ch = GLYPN(cnt->info)->ch;
		printf("\e[%d;%dm%c\e[0m", bold, color, ch);
		cnt = cnt->next;
	}
	int cursor_pad = getCursorRightPadding(line);
	if (cursor_pad > 0)
		CURSOR_LEFT(cursor_pad);
}

void LineCursorRight(Line *line, u8 n)
{
	for (int i = 0; line->cursor->next && i < n; i++)
		line->cursor = line->cursor->next;
}

void LineCursorLeft(Line *line, u8 n)
{
	for (int i = 0; line->cursor->pre && i < n; i++)
		line->cursor = line->cursor->pre;
}

void LineFree(Line *line)
{
	dlist_clear(&line->glpyns, free);
	line->cursor = NULL;
}

char *LineToStr(Line *line)
{
	StringBuffer *sb = StringBufferCreate();
	dlist_t *cnt = line->glpyns->next;

	while (cnt != NULL) {
		StringBufferAppend(sb, GLYPN(cnt->info)->ch);
		cnt = cnt->next;
	}

	return StringBufferToString(sb, true);
}
