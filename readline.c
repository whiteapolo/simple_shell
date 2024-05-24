#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "completion.h"
#include "history.h"
#include "readline.h"
#include "data_structers/mystrings.h"
#include "data_structers/trie.h"
#include "line.h"
#include "mycursors.h"
#include "types.h"

static Trie *g_syntax_trie = NULL;
static HistoryEntry *g_hist_entry = NULL;
static void (*g_keys_map[150])(Line *line, u16 key) = {0};

u16 getch();
void binding(Line *line, u16 key);
void bindingReturn(Line *line, u16 key);
void bindingSpace(Line *line, u16 key);
void bindingArrowUp(Line *line, u16 key);
void bindingArrowDown(Line *line, u16 key);
void bindingArrowRight(Line *line, u16 key);
void bindingArrowLeft(Line *line, u16 key);
void bindingControlL(Line *line, u16 key);
void bindingTab(Line *line, u16 key);

void ReadLineInit(HistoryEntry *hist_entry, Trie *syntax_trie)
{
	CURSOR_TO_BLOCK();
	for (u8 i = '!'; i <= '~'; i++)
		g_keys_map[i] = binding;
	g_keys_map[KEY_RETURN]    		= bindingReturn;
	g_keys_map[KEY_SPACE]     		= bindingSpace;
	g_keys_map[KEY_ARROW_UP]  		= bindingArrowUp;
	g_keys_map[KEY_ARROW_DOWN]		= bindingArrowDown;
	g_keys_map[KEY_ARROW_RIGHT] 	= bindingArrowRight;
	g_keys_map[KEY_ARROW_LEFT] 	= bindingArrowLeft;
	g_keys_map[KEY_CONTROL_L] 		= bindingControlL;
	g_keys_map[KEY_TAB] 		= bindingTab;
	g_syntax_trie = syntax_trie;
	g_hist_entry = hist_entry;
}

void printLine(Line *line, const char *prompt)
{
	CLEAR_LINE();
	CURSOR_COL(0);
	printf("%s", prompt);
	if (g_syntax_trie)
		LineHighlight(line, g_syntax_trie);
	LinePrint(line);
	LineClearHighlight(line);
	fflush(stdout);
}

char *ReadLine(const char *prompt)
{
	u16 key;
	Line line;
	LineInit(&line);

	printLine(&line, prompt);
	while ((key = getch()) != '\n') {
		// if its a binding call the binding
		if (g_keys_map[key])
			g_keys_map[key](&line, key);
		printLine(&line, prompt);
	}

	putchar('\n');

	clear_completions();
	char *str = LineToStr(&line);
	LineFree(&line);
	return str;
}

u16 getch()
{
        u16 buf = 0;
        struct termios old = {0};
        tcgetattr(0, &old);
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, &old);
        IGNORE(read(0, &buf, 1));
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        tcsetattr(0, TCSADRAIN, &old);

	// if its an arrow
	if (buf == 27) {
        	IGNORE(read(0, &buf, 1));
        	IGNORE(read(0, &buf, 1));
		buf += 63;
	}

        return buf;
}

void binding(Line *line, u16 key)
{
	LineAppend(line, key, RESET);
}

void bindingReturn(Line *line, u16 key)
{
	LineDelete(line);
}

void bindingSpace(Line *line, u16 key)
{
	LineAppend(line, ' ', RESET);
}

void bindingArrowUp(Line *line, u16 key)
{
	const char *command = HistoryBackwars(g_hist_entry);
	if (command != NULL)
		LineSetString(line, command, 0);
}

void bindingArrowDown(Line *line, u16 key)
{
	const char *command = HistoryFoward(g_hist_entry);
	if (command != NULL)
		LineSetString(line, command, 0);
}

void bindingArrowRight(Line *line, u16 key)
{
	LineCursorRight(line, 1);
}

void bindingArrowLeft(Line *line, u16 key)
{
	LineCursorLeft(line, 1);
}

void bindingControlL(Line *line, u16 key)
{
	CLEAR_SCREEN();
	fflush(stdout);
}

void bindingTab(Line *line, u16 key)
{
	if (g_syntax_trie != NULL)
		LineComplete(line, g_syntax_trie);
}
