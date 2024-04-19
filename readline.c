#include "readline.h"

// Implementation
static char *buffer;
static int buffer_index;
static int buffer_size;
static const char *(*syntax_highlighting_func)(const char *line);
static int cursor_dx;

void action_tab(int val);
void action_control_l(int val);
void action_arrow_left(int val);
void action_arrow_right(int val);
void action_arrow_up(int val);
void action_space(int val);
void action_return(int val);
void action_regular_key(int val);
void action_arrow_down(int val);

#define KEYS_MAP_SIZE 150

static void (*keys_functions_map[KEYS_MAP_SIZE])(int key) = {
	[0 ... KEYS_MAP_SIZE - 1] = NULL,
	[KEY_RETURN] = action_return,
	[KEY_SPACE] = action_space,
	[KEY_ARROW_UP] = action_arrow_up,
	[KEY_ARROW_DOWN] = action_arrow_down,
	[KEY_ARROW_RIGHT] = action_arrow_right,
	[KEY_ARROW_LEFT] = action_arrow_left,
	[KEY_CONTROL_L] = action_control_l,
	[KEY_TAB] = action_tab,
	['!' ... '~'] = action_regular_key,
};


int getch();

/* func can be NULL */
void ReadLineInit(const char *(*syntax_func)(const char *line))
{
	cursor_dx = 0;
	buffer_size = 256;
	buffer_index = 0;
	buffer = MALLOC(char, buffer_size);
	*buffer = 0;
	syntax_highlighting_func = syntax_func;
}

char *ReadLine(const char *promt)
{
	int promt_len = strlen(promt);
	printf("%s%s", promt, buffer);
	fflush(stdout);

	int key = getch();
	while (key != '\n') {
		if (keys_functions_map[key])
			keys_functions_map[key](key);
		CLEAR_LINE();
		CURSOR_LEFT(buffer_index + promt_len);
		printf("%s%s", promt, buffer);
		fflush(stdout);
		if (buffer_index + 10 > buffer_size) {
			buffer_size += 50;
			buffer = REALLOC(buffer, char, buffer_size);
		}
		key = getch();
	}
	putchar('\n');
	char *tmp = strdup(buffer);
	buffer_index = 0;
	*buffer = 0;
	return tmp;
}

void ReadLineDestroy()
{
	free(buffer);
}


int getch()
{
        int buf = 0;
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

	if (buf == 27) {
        	read(0, &buf, 1);
        	read(0, &buf, 1);
		buf += 63;
	}

        return (buf);
}

void action_regular_key(int val)
{
	buffer[buffer_index++] = val;
	buffer[buffer_index] = 0;
}

void action_return(int val)
{
	if (buffer_index > 0) {
		buffer[--buffer_index] = 0;
		CURSOR_LEFT(1);
	}
}

void action_space(int val)
{
	buffer[buffer_index++] = ' ';
	buffer[buffer_index] = 0;
}

void action_arrow_up(int val)
{

}

void action_arrow_down(int val)
{

}

void action_arrow_right(int val)
{
	
}
void action_arrow_left(int val)
{
	cursor_dx++;
}

void action_control_l(int val)
{
	fputs("\e[1;1H\e[2J", stdout);
	fflush(stdout);
}

void action_tab(int val)
{

}
