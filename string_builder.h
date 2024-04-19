#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdbool.h>
#include "data_structers/lll.h"

typedef struct {
	lll_t *start;
	lll_t *end;
	int size;
} StringBuilder;

void StringBuilderInit(StringBuilder **sb);
void StringBuilderAppend(StringBuilder *sb, const char *str);
void StringBuilderAppendIndex(StringBuilder *sb, const char *str, int n);
void StringBuilderRemoveIndex(StringBuilder *sb, int n);
const char *StringBuilderGet(StringBuilder *sb, int n);
char **StringBuilderToStrings(StringBuilder *sb, bool destroySb);
char *StringBuilderToString(StringBuilder *sb, const char *delim, bool destroySb);
void StringBuilderDestroy(StringBuilder *sb);

char *strcut(const char *str, const char *sub, const char *by);

#endif
