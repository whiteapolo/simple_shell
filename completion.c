#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "completion.h"
#include "data_structers/mystrings.h"
#include "path.h"
#include "data_structers/trie.h"
#include "types.h"
#include <wh/messure_time.h>

char **complete_program(const char *str, int *matches, Trie *syntax_trie)
{
	return TrieGetAllMatches(syntax_trie, str, matches);
}

char **complete_path(const char *str, int *matches)
{
	int len = strlen(str);
	char *dir;
	if (strchr(str, '/')) {
		while (len > 1 && str[len] != '/') {
			len--;
		}
		len++;
		dir = strndup(str, len);
	} else {
		dir = strdup(".");
		len = 0;
	}
	Trie trie;
	TrieInit(&trie);
	FOR_EACH_FILE_IN_DIR(dir, path,
		char *str;
		if (is_dir(path))
			str = str2dup(path, "/");
		else
			str = strdup(path);
		TrieAdd(&trie, str);
		free(str);
	);
	char **completions = TrieGetAllMatches(&trie, str + len, matches);
	TrieDestroy(&trie);
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
