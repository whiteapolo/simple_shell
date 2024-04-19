#ifndef DISPLAY_H
#define DISPLAY_H

/*********************************************************************
 *                      CURSOR MANIPULATION                          *
 ********************************************************************/

#define SAVE_CURSOR() 		fputs("\033[s", stdout)
#define RESTORE_CURSOR() 	fputs("\033[u", stdout)

#define CLEAR_SCREEN() 		fputs("27[2K", stdout)
#define CLEAR_LINE() 		fputs("\033[K", stdout)

#define CURSOR_TO(row, cols) 	printf("\x1B[%d;%dH", rows, cols)

#define CURSOR_DOWN(rows) 	printf("\033[%dB", rows)
#define CURSOR_UP(rows) 	printf("\033[%dA", rows)
#define CURSOR_RIGHT(cols) 	printf("\033[%dC", cols)
#define CURSOR_LEFT(cols) 	printf("\033[%dD", cols)

// Hide the cursor (alternative method)
#define CURSOR_HIDE() 		fputs("\033[?25l")

// Show the cursor (alternative method)
#define CURSOR_SHOW() 		fputs("\033[?25h")

#define CURSOR_TO_BLOCK() 	fputs("\e[2 q", stdout)

/*********************************************************************
 *                      TEXT FORMATTING                              *
 ********************************************************************/

#define SET_BOLD_TEXT() 	fputs("\033[1m", stdout)
#define SET_NORMAL_TEXT() 	fputs("\033[22m", stdout)
#define RESET_ATTRIBUTES() 	fputs("\033[0m", stdout)


/*********************************************************************
 *                        TEXT COLORS                                *
 ********************************************************************/

// Set text color to a custom RGB value
#define SET_TEXT_COLOR_RGB(r, g, b) 	printf("\033[38;2;%d;%d;%dm", r, g, b)
	
#define SET_DEFAULT_TEXT_COLOR() 	fputs("\033[0m", stdout)
#define SET_TEXT_COLOR_RED() 		fputs("\033[31m", stdout)
#define SET_TEXT_COLOR_GREEN() 	fputs("\033[32m")
#define SET_TEXT_COLOR_YELLOW() 	fputs("\033[33m", stdout)
#define SET_TEXT_COLOR_BLUE() 		fputs("\033[34m", stdout)
#define SET_TEXT_COLOR_MAGENTA() 	fputs("\033[35m", stdout)
#define SET_TEXT_COLOR_CYAN() 		fputs("\033[36m", stdout)
#define SET_TEXT_COLOR_WHITE() 		fputs("\033[37m", stdout)


/*********************************************************************
 *                      BACKGROUND COLORS                            *
 ********************************************************************/
// Set background color to a custom RGB value
#define SET_BG_COLOR_RGB(r, g, b) fputs("\033[48;2;%d;%d;%dm", r, g, b)

#define SET_DEFAULT_BG_COLOR() 	fputs("\033[49m")
#define SET_BG_COLOR_BLACK_BRIGHT() 	fputs("\033[40;1m", stdout)
#define SET_BG_COLOR_RED_BRIGHT() 	fputs("\033[41;1m", stdout)
#define SET_BG_COLOR_GREEN_BRIGHT() 	fputs("\033[42;1m", stdout)
#define SET_BG_COLOR_YELLOW_BRIGHT() 	fputs("\033[43;1m", stdout)
#define SET_BG_COLOR_BLUE_BRIGHT() 	fputs("\033[44;1m", stdout)
#define SET_BG_COLOR_MAGENTA_BRIGHT() 	fputs("\033[45;1m", stdout)
#define SET_BG_COLOR_CYAN_BRIGHT() 	fputs("\033[46;1m", stdout)
#define SET_BG_COLOR_WHITE_BRIGHT() 	fputs("\033[47;1m", stdout)


/*********************************************************************
 *                           TERMINAL                                *
 ********************************************************************/


/* struct winsize w */
/* w.ws_row 
 * w.ws_col */
#define TERMINAL_DIMENTIONS(w) ioctl(0, TIOCGWINSZ, &w)

#endif
