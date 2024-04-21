#ifndef CONFIG_H
#define CONFIG_H

static const char *promt = " $ ";

static int history_size = 500;

static char *env[] = {
	"", /* saved for path */
/* #include "env.agl" */
	NULL,
};

static const char *g_path[] = {
	"/bin/",
	"/usr/local/bin/",
};

static char *aliases[][2] = {
	{"vim", "nvim"},
	{"m", "make"},
	{"e", "./exe"},
};

#endif
