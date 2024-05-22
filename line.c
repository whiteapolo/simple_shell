#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <wh/messure_time.h>
#include <stdbool.h>

#include "completion.h"
#include "data_structers/trie.h"
#include "line.h"
#include "data_structers/dict.h"
#include "data_structers/mystrings.h"
#include "mycursors.h"
#include "types.h"
#include "data_structers/double_lll.h"
#include "path.h"

#define GLYPN(glpyn) ((Glypn*)(glpyn))

Glypn *makeGlypn(char ch, u8 color, bool bold);

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

void LineSetColor(Line *line, u8 color)
{
	dlist_t *cnt = line->glpyns->next;
	while (cnt != NULL) {
		GLYPN(cnt->info)->color = color;
		cnt = cnt->next;
	}
}

void LinePrint(Line *line)
{
	FOR_EACH_NODE(line->glpyns->next, info, 
		u8 bold = GLYPN(*info)->bold;
		u8 color = GLYPN(*info)->color;
		u8 ch = GLYPN(*info)->ch;
		printf("\e[%d;%dm%c\e[0m", bold, color, ch);
	);
	int cursor_right_pad = -1;
	FOR_EACH_NODE(line->cursor, info,
		cursor_right_pad++;
	);
	if (cursor_right_pad > 0)
		CURSOR_LEFT(cursor_right_pad);
}

void LineFree(Line *line)
{
	dlist_clear(&line->glpyns, free);
	line->cursor = NULL;
}

char *LineToStr(Line *line)
{
	Sbuffer *sb = SbufferCreate();

	FOR_EACH_NODE(line->glpyns->next, info,
		SbufferAppend(sb, GLYPN(*info)->ch);
	);

	return SbufferToString(sb, true);
}

char *wordTokFoward(dlist_t **lst)
{
	Sbuffer *sb = SbufferCreate();
	while (*lst && GLYPN((*lst)->info)->ch != ' ') {
		SbufferAppend(sb, GLYPN((*lst)->info)->ch);
		*lst = (*lst)->next;
	}

	while (*lst && GLYPN((*lst)->info)->ch == ' ') {
		*lst = (*lst)->next;
	}

	char *word = SbufferToString(sb, true);
	if (IS_EMPTY_STRING(word)) {
		free(word);
		word = NULL;
	}
	return word;
}

char *wordTokBackwards(dlist_t **lst)
{
	dlist_t *cnt = *lst;
	while (cnt->pre != NULL && GLYPN(cnt->info)->ch != ' ')
		cnt = cnt->pre;
	cnt = cnt->next;
	*lst = cnt;
	return wordTokFoward(&cnt);
}

void HighlightWord(dlist_t *word, u8 color, bool bold)
{
	while (word && GLYPN(word->info)->ch != ' ') {
		GLYPN(word->info)->color = color;
		GLYPN(word->info)->bold = bold;
		word = word->next;
	}
}

void LineHighlight(Line *line, Trie *syntax_trie)
{
	LineClearHighlight(line);
	dlist_t *cnt = line->glpyns->next;
	dlist_t *before = cnt;
	char *word = wordTokFoward(&cnt);
	int word_count = 0;

	while (word != NULL) {
		if (word[0] == '-')
			HighlightWord(before, CYAN, false);
		else if (isFileExists(word))
			HighlightWord(before, MAGENTA, false);
		else if (TrieExists(syntax_trie, word) && word_count == 0)
			HighlightWord(before, GREEN, false);
		else if (word_count == 0)
			HighlightWord(before, RED, true);
		free(word);
		before = cnt;
		word = wordTokFoward(&cnt);
		word_count++;
	}
}

void LineClearHighlight(Line *line)
{
	LineSetColor(line, 0);
}

void showCompletions(char **completions, int size)
{
	const int maxSize = MIN(MIN(7, size), TERMINAL_COL());
	int i = 0;
	while (i < maxSize) {
		putchar('\n');
		i++;
	}
	CURSOR_UP(i);
	SAVE_CURSOR();
	FOR(i, maxSize) {
		printf("\n%s", completions[i]);
	}
	RESTORE_CURSOR();
}

void clearCompletions()
{
	SAVE_CURSOR();
	const int rows = TERMINAL_ROW();
	FOR(i, rows) {
		CURSOR_DOWN(1);
		CLEAR_LINE();
	}
	RESTORE_CURSOR();
}

void LineComplete(Line *line, Trie *syntax_trie)
{
	dlist_t *cursor = line->cursor;

	if (cursor->pre == NULL) return;
	/* if (GLYPN(cursor->info)->ch == ' ') return; */
	/* if (cursor->pre->info != NULL && GLYPN(cursor->pre->info)->ch == ' ') return; */

	int matches;
	char **completions = complete_line(LineToStr(line), &matches, syntax_trie);
	if (*completions) {
		LineAppendString(line, *completions, 0);
	}
	if (matches > 2) {
		clearCompletions();
		showCompletions(completions + 1, matches - 1);
	}

	splitfree(completions);
}

Glypn *makeGlypn(char ch, u8 color, bool bold)
{
	Glypn *glpyn = malloc(sizeof(Glypn));
	glpyn->ch = ch;
	glpyn->color = color;
	glpyn->bold = bold;
	return glpyn;
}
