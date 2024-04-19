#include "string_builder.h"
#include "data_structers/lll.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <wh/macros.h>

void StringBuilderInit(StringBuilder **sb)
{
	*sb = MALLOC(StringBuilder, 1);
	lll_init(&(*sb)->start);
	(*sb)->size = 0;
}

void StringBuilderAppend(StringBuilder *sb, const char *str)
{
	if (sb->size == 0) {
		lll_push(&sb->start, strdup(str));
		sb->end = sb->start;
	} else {
		lll_insert_after(sb->end, strdup(str));
		sb->end = sb->end->next;
	}
	sb->size++;
}

void StringBuilderAppendIndex(StringBuilder *sb, const char *str, int n)
{
	lll_t *cnt = sb->start;
	if (sb->size == n) {
		StringBuilderAppend(sb, str);
	} else if (n == 0) {
		lll_push(&sb->start, strdup(str));
		sb->size++;
	} else {
		FOR(i, n-1)
			cnt = cnt->next;
		lll_insert_after(cnt, strdup(str));
		sb->size++;
	}
}

void StringBuilderRemoveIndex(StringBuilder *sb, int n)
{
	void *tmp;
	if (n == 0) {
		tmp = lll_pop(&sb->start);
	} else {
		lll_t *cnt = sb->start;
		FOR(i, n-1)
			cnt = cnt->next;
		tmp = lll_remove_after(cnt);
	}
	if (tmp)
		free(tmp);
	sb->size--;
}

const char *StringBuilderGet(StringBuilder *sb, int n)
{
	lll_t *cnt = sb->start;
	FOR(i, n)
		cnt = cnt->next;
	return cnt->info;
}

/* end is marked with null pointer */
char **StringBuilderToStrings(StringBuilder *sb, bool destroySb)
{
	char **s = MALLOC(char*, sb->size + 1);
	lll_t *cnt = sb->start;
	int i = 0;
	FOR(i, sb->size) {
		s[i] = strdup(cnt->info);
		cnt = cnt->next;
	}
	s[sb->size] = NULL;
	if (destroySb)
		StringBuilderDestroy(sb);
	return s;
}

char *StringBuilderToString(StringBuilder *sb, const char *delim, bool destroySb)
{
	char *s = NULL;
	int size = 1;
	lll_t *cnt = sb->start;
	int delim_size = strlen(delim);
	int slen;

	FOR(i, sb->size) {
		slen = strlen(cnt->info);
		size += slen + delim_size;
		s = REALLOC(s, char, size);
		strcat(s + size - (slen + delim_size) - 1, cnt->info);
		strcat(s + size - delim_size - 1, delim);
		cnt = cnt->next;
	}

	s[size - delim_size - 1] = '\0';

	if (destroySb)
		StringBuilderDestroy(sb);
	return s;
}

void StringBuilderDestroy(StringBuilder *sb)
{
	if (sb->start != NULL)
		lll_clear(&sb->start, free);
	free(sb);
}

/* replace a substring with another substring */
char *strcut(const char *str, const char *sub, const char *by)
{

	if (!strcmp(str, sub)) {
		return strdup(by);
	}

	const char *start = strstr(str, sub);
	if (start == NULL)
		return NULL;

	const char *end = start + strlen(sub);

	int size = strlen(str) - strlen(sub) + strlen(by) + 1;
	char *dest = MALLOC(char, size);
	char *dest_start = dest;

	while (str != start) {
		*(dest++) = *(str++);
	}

	while (*by) {
		*(dest++) = *(by++);
	}

	str = end;

	while (*str) {
		*(dest++) = *(str++);
	}

	return dest_start;
}
