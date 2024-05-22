#ifndef MY_CURSORS_H
#define MY_CURSORS_H

#include <sys/ioctl.h>
#include <stdio.h>

/*********************************************************************
 *                      CURSOR MANIPULATION                          *
 ********************************************************************/

#define SAVE_CURSOR() 		fputs("\e[s", stdout)
#define RESTORE_CURSOR() 	fputs("\e[u", stdout)

#define CLEAR_SCREEN() 		fputs("\e[1;1H\e[2J", stdout);
#define CLEAR_LINE() 		fputs("\e[2K", stdout)

#define CURSOR_TO(row, cols) 	printf("\x1B[%d;%dH", rows, cols)
#define CURSOR_COL(col) 	printf("\e[%dG", col)

#define CURSOR_DOWN(rows) 	printf("\e[%dB", rows)
#define CURSOR_UP(rows) 	printf("\e[%dA", rows)
#define CURSOR_RIGHT(cols) 	printf("\e[%dC", cols)
#define CURSOR_LEFT(cols) 	printf("\e[%dD", cols)

// Hide the cursor (alternative method)
#define CURSOR_HIDE() 		fputs("\e[?25l")

// Show the cursor (alternative method)
#define CURSOR_SHOW() 		fputs("\e[?25h")

#define CURSOR_TO_BLOCK() 	fputs("\e[2 q", stdout)

/*********************************************************************
 *                      TEXT FORMATTING                              *
 ********************************************************************/

#define SET_BOLD_TEXT() 	fputs("\e[1m", stdout)
#define SET_NORMAL_TEXT() 	fputs("\e[22m", stdout)
#define RESET_ATTRIBUTES() 	fputs("\e[0m", stdout)


/*********************************************************************
 *                        TEXT COLORS                                *
 ********************************************************************/

// Set text color to a custom RGB value
#define SET_TEXT_COLOR_RGB(r, g, b) 	printf("\e[38;2;%d;%d;%dm", r, g, b)
	
#define SET_DEFAULT_TEXT_COLOR() 	fputs("\e[0m", stdout)
#define SET_TEXT_COLOR_RED() 		fputs("\e[31m", stdout)
#define SET_TEXT_COLOR_GREEN() 	fputs("\e[32m")
#define SET_TEXT_COLOR_YELLOW() 	fputs("\e[33m", stdout)
#define SET_TEXT_COLOR_BLUE() 		fputs("\e[34m", stdout)
#define SET_TEXT_COLOR_MAGENTA() 	fputs("\e[35m", stdout)
#define SET_TEXT_COLOR_CYAN() 		fputs("\e[36m", stdout)
#define SET_TEXT_COLOR_WHITE() 		fputs("\e[37m", stdout)


/*********************************************************************
 *                      BACKGROUND COLORS                            *
 ********************************************************************/
// Set background color to a custom RGB value
#define SET_BG_COLOR_RGB(r, g, b) fputs("\e[48;2;%d;%d;%dm", r, g, b)

#define SET_DEFAULT_BG_COLOR() 	fputs("\e[49m")
#define SET_BG_COLOR_BLACK_BRIGHT() 	fputs("\e[40;1m", stdout)
#define SET_BG_COLOR_RED_BRIGHT() 	fputs("\e[41;1m", stdout)
#define SET_BG_COLOR_GREEN_BRIGHT() 	fputs("\e[42;1m", stdout)
#define SET_BG_COLOR_YELLOW_BRIGHT() 	fputs("\e[43;1m", stdout)
#define SET_BG_COLOR_BLUE_BRIGHT() 	fputs("\e[44;1m", stdout)
#define SET_BG_COLOR_MAGENTA_BRIGHT() 	fputs("\e[45;1m", stdout)
#define SET_BG_COLOR_CYAN_BRIGHT() 	fputs("\e[46;1m", stdout)
#define SET_BG_COLOR_WHITE_BRIGHT() 	fputs("\e[47;1m", stdout)

static int TERMINAL_COL()
{
	struct winsize w; 
	ioctl(0, TIOCGWINSZ, &w);
	return w.ws_col;
}

static int TERMINAL_ROW()
{
	struct winsize w; 
	ioctl(0, TIOCGWINSZ, &w);
	return w.ws_row;
}

/*********************************************************************
 *                           TERMINAL                                *
 ********************************************************************/


/* struct winsize w */
/* w.ws_row 
 * w.ws_col */
#define TERMINAL_DIMENTIONS(w) ioctl(0, TIOCGWINSZ, &w)

#endif
