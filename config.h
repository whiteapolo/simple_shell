#ifndef CONFIG_H
#define CONFIG_H

static const char *promt = " \e[0;32m$\e[0m ";

// history
static const char *history_file = "/home/white/.config/sh/history";
static const int history_size = 1000;

static char *env[] = {
	"SHELL=/usr/bin/zsh",
	"EDITOR=nvim",
	"CPLUS_INCLUDE_PATH=/usr/include/c++/11",
};

static const char *path[] = {
	"/home/white/.cargo/bin",
	"/bin",
	"/usr/bin",
	"/usr/local/bin",
	"/usr/sbin",
	"/home/white/.local/bin",
	"/home/white/archive/scripts",
};

static char *aliases[][2] = {
	{ "vimrc", "cd ~/.config/nvim" },
	{ "swap", "cd ~/.local/state/nvim/swap" },
	{ "du", "du -h" },
	{ "apt", "sudo apt" },
	{ "se", "grep -ri --color=always" },
	{ "grep", "grep --color=always" },
	{ "rm", "rm -i" },
	{ "p", "printenv PATH" },
	{ "..", "cd .." },
	{ "vim", "nvim" },
	{ "m", "./build.sh" },
	{ "e", "./exe" },
	{ "ls", "ls --color --group-directories-first" },
	{ "ll", "ls -la --color --group-directories-first" },
	{ "l", "ls -la --color --group-directories-first" },
	{ "fl", "nnn -de" },
};

#endif
