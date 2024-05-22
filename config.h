#ifndef CONFIG_H
#define CONFIG_H

static const char *promt = " \e[0;32m$\e[0m ";

// history
static const char *history_file = "/home/white/.config/sh/history";
static const int history_size = 1000;

static const char *path[] = {
	"/bin",
	"/usr/bin",
	"/usr/local/bin",
	"/usr/sbin",
	"/home/white/.local/bin",
	"/home/white/archive/scripts",
};

static char *aliases[][2] = {
	{ "p", "printenv PATH" },
	{ "..", "cd .." },
	{ "vim", "nvim" },
	{ "m", "./build.sh" },
	{ "e", "./exe" },
	{ "ls", "ls --color --group-directories-first" },
	{ "ll", "ls -la --color" },
	{ "l", "ls -la --color" },
};

#endif
