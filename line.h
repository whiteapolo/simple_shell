#ifndef LINE_H
#define LINE_H

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "data_structers/double_lll.h"
#include "data_structers/dict.h"
#include "data_structers/trie.h"

#define RESET 		0     /*  RESET   */

#define RED 		31    /*  RED     */
#define GREEN 		32    /*  GREEN   */
#define YELLOW 		33    /*  YELLOW  */
#define BLUE 		34    /*  BLUE    */
#define MAGENTA 	35    /*  MAGENTA */
#define CYAN 		36    /*  CYAN    */
#define WHITE 		37    /*  WHITE   */
#define GRAY 		90    /*  GRAY    */

#define BRIGHT_RED 	91   /*  BRIGHT RED     */
#define BRIGHT_GREEN 	92   /*  BRIGHT GREEN   */
#define BRIGHT_YELLOW 	93   /*  BRIGHT YELLOW  */
#define BRIGHT_BLUE 	94   /*  BRIGHT BLUE    */
#define BRIGHT_MAGENTA 	95   /*  BRIGHT MAGENTA */
#define BRIGHT_CYAN 	96   /*  BRIGHT CYAN    */
#define BRIGHT_WHITE 	97   /*  BRIGHT WHITE   */
#define BRIGHT_GRAY 	90   /*  BRIGHT GRAY    */

#define RESET_COLOR 		"\e[0;0m"     /*  RESET   */

#define RED_COLOR 		"\e[0;31m"   /*  RED     */
#define GREEN_COLOR 		"\e[0;32m"   /*  GREEN   */
#define YELLOW_COLOR 		"\e[0;33m"   /*  YELLOW  */
#define BLUE_COLOR 		"\e[0;34m"   /*  BLUE    */
#define MAGENTA_COLOR 	 	"\e[0;35m"   /*  MAGENTA */
#define CYAN_COLOR 		"\e[0;36m"   /*  CYAN    */
#define WHITE_COLOR 		"\e[0;37m"   /*  WHITE   */
#define GRAY_COLOR 		"\e[0;90m"   /*  GRAY    */

#define BRIGHT_RED_COLOR 	"\e[0;91m"   /*  BRIGHT RED     */
#define BRIGHT_GREEN_COLOR 	"\e[0;92m"   /*  BRIGHT GREEN   */
#define BRIGHT_YELLOW_COLOR 	"\e[0;93m"   /*  BRIGHT YELLOW  */
#define BRIGHT_BLUE_COLOR 	"\e[0;94m"   /*  BRIGHT BLUE    */
#define BRIGHT_MAGENTA_COLOR 	"\e[0;95m"   /*  BRIGHT MAGENTA */
#define BRIGHT_CYAN_COLOR 	"\e[0;96m"   /*  BRIGHT CYAN    */
#define BRIGHT_WHITE_COLOR 	"\e[0;97m"   /*  BRIGHT WHITE   */
#define BRIGHT_GRAY_COLOR 	"\e[0;90m"   /*  BRIGHT GRAY    */



typedef struct {
	char ch;
	bool bold;
	u8 color;
} Glypn;

typedef struct {
	dlist_t *glpyns;
	dlist_t *cursor;
} Line;


void LineInit(Line *line);
void clearCompletions();
void LineAppend(Line *line, char ch, u8 color);
void LineAppendString(Line *line, const char *str, u8 color);
void LineDelete(Line *line);
void LineClear(Line *line);
void LinePrint(Line *line);
void LineFree(Line *line);
char *LineToStr(Line *line);
void LineHighlight(Line *line, Trie *syntax_trie);
void LineClearHighlight(Line *line);
void LineCursorRight(Line *line, u8 n);
void LineCursorLeft(Line *line, u8 n);
void LineComplete(Line *line, Trie *syntax_trie);
void LineSetString(Line *line, const char *str, u8 color);

#endif
