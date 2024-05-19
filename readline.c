#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "readline.h"
#include "data_structers/string_builder.h"
#include "data_structers/trie.h"
#include "line.h"
#include "mycursors.h"

static Trie *g_syntax_trie = NULL;
static void (*keysmap[150])(Line *line, u16 key) = {0};
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

void ReadLineInit()
{
	for (u8 i = '!'; i < '~'; i++)
		keysmap[i] = binding;
	keysmap[KEY_RETURN]    		= bindingReturn;
	keysmap[KEY_SPACE]     		= bindingSpace;
	keysmap[KEY_ARROW_UP]  		= bindingArrowUp;
	keysmap[KEY_ARROW_DOWN]		= bindingArrowDown;
	keysmap[KEY_ARROW_RIGHT] 	= bindingArrowRight;
	keysmap[KEY_ARROW_LEFT] 	= bindingArrowLeft;
	keysmap[KEY_CONTROL_L] 		= bindingControlL;
	keysmap[KEY_TAB] 		= bindingTab;
}

void printLine(Line *line, const char *promt, Dict *highlight_dict)
{
	CLEAR_LINE();
	CURSOR_COL(0);
	printf("%s", promt);
	if (highlight_dict)
		LineHighlight(line, highlight_dict);
	LinePrint(line);
	LineClearHighlight(line);
	fflush(stdout);
}
void ReadLineSetSyntaxTrie(Trie *syntax_trie)
{
	g_syntax_trie = syntax_trie;
}

char *ReadLine(const char *promt, Dict *highlight_dict)
{
	u16 key;
	Line line;
	LineInit(&line);

	printLine(&line, promt, highlight_dict);
	while ((key = getch()) != '\n') {
		// if its a binding call the binding
		if (keysmap[key])
			keysmap[key](&line, key);
		printLine(&line, promt, highlight_dict);
	}

	putchar('\n');

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
        read(0, &buf, 1);
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        tcsetattr(0, TCSADRAIN, &old);

	// if its an arrow
	if (buf == 27) {
        	read(0, &buf, 1);
        	read(0, &buf, 1);
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

}

void bindingArrowDown(Line *line, u16 key)
{
	LineSetString(line, "itay mehadab", YELLOW);
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
		LineMatchCursor(line, g_syntax_trie);
}
