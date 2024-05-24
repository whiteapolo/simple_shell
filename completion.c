#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "completion.h"
#include "data_structers/mystrings.h"
#include "path.h"
#include "data_structers/trie.h"
#include "types.h"
#include "mycursors.h"
#include "line.h"

int cmp_files(const void *a, const void *b)
{
	const char **str1 = (const char **)a;
	const char **str2 = (const char **)b;
	if ((is_dir(*str1) && is_dir(*str2)))
		return 0;
	else if (is_dir(*str1))
		return -1;
	else if (is_dir(*str2))
		return 1;
	else
		return 0;
}

void show_completions(char **completions, int size)
{
	qsort(completions, size, sizeof(*completions), cmp_files);
	const int maxSize = MIN(MIN(20, size), TERMINAL_COL());
	int i = 0;
	while (i < maxSize) {
		putchar('\n');
		i++;
	}
	CURSOR_UP(i);
	SAVE_CURSOR();
	FOR(i, maxSize) {
		if (is_dir(completions[i]))
			printf("\n" BRIGHT_BLUE_COLOR "%s" RESET_COLOR, completions[i]);
		else
			printf("\n%s", completions[i]);
	}
	RESTORE_CURSOR();
}

void clear_completions()
{
	SAVE_CURSOR();
	const int rows = TERMINAL_ROW();
	FOR(i, rows) {
		CURSOR_DOWN(1);
		CLEAR_LINE();
	}
	RESTORE_CURSOR();
}

char **complete_program(const char *str, int *matches, Trie *syntax_trie)
{
	return TrieGetAllMatches(syntax_trie, str, matches);
}

char **complete_path(const char *str, int *matches)
{
	char *unpritty_str = strdup(str);
	unprettify_path(&unpritty_str);
	int len = strlen(str);
	const char *devidor = strrchr(unpritty_str, '/');
	char *parent_dir;
	char *file_name;

	if (devidor) {
		strdevide(unpritty_str, devidor - unpritty_str, &parent_dir, &file_name);
	} else {
		parent_dir= strdup("./");
		file_name = strdup(unpritty_str);
	}

	Trie trie;
	TrieInit(&trie);
	FOR_EACH_FILE_IN_DIR(parent_dir, path, *file_name == '.',
		char *str = strdup(path);
		char *allpath = str2dup(parent_dir, str);
		if (is_dir(allpath))
			strpush(&str, '/');
		TrieAdd(&trie, str);
		free(str);
		free(allpath);
	);

	char **completions = TrieGetAllMatches(&trie, file_name, matches);
	TrieDestroy(&trie);
	free(file_name);
	free(parent_dir);
	free(unpritty_str);
	return completions;
}

/* bool check_regex(const char *reg, const char *str) */
/* { */
/* 	regex_t regex; */
/* 	regcomp(&regex, reg, 0); */
/* 	bool result = !regexec(&regex, str, 0, NULL, 0); */
/* 	regfree(&regex); */
/* 	return result; */
/* } */

char **complete_line(const char *line, int *matches, Trie *syntax_trie)
{
	while (*line == ' ')
		line++;
	const char *start = line + strlen(line);
	while (start != line && *start != ' ')
		start--;

	if (*start == ' ')
		start++;

	if (start != line) {
		return complete_path(start, matches);
	} else {
		return complete_program(start, matches, syntax_trie);
	}
}
