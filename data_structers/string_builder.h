#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdbool.h>
#include <string.h>

typedef struct {
	char **data;
	int size;
} StringBuilder;

typedef struct {
	char *data;
	int size;
} StringBuffer;

#define IS_EMPTY_STRING(str) (!(str && (*str)))
#define CHAR_TO_STR(c) ((char[]){c, '\0'})

StringBuilder *StringBuilderCreate();
void StringBuilderAppend(StringBuilder *sb, const char *str);
void StringBuilderAppendIndex(StringBuilder *sb, const char *str, int n);
void StringBuilderRemoveIndex(StringBuilder *sb, int n);
const char *StringBuilderGet(StringBuilder *sb, int n);
char **StringBuilderToStrings(StringBuilder *sb, bool destroySb);
char *StringBuilderToString(StringBuilder *sb, const char *delim, bool destroySb);
void StringBuilderDestroy(StringBuilder *sb);
void StringBuilderConcat(StringBuilder *sb1, StringBuilder *sb2);
StringBuilder *StringBuilderDup(StringBuilder *sb);
StringBuilder *StringBuilderCreateFromStrings(char **str);
StringBuilder *StringBuilderCreateFromString(const char *str, const char *delim);

StringBuffer *StringBufferCreate();
void StringBufferAppend(StringBuffer *sb, char c);
char StringBufferRemove(StringBuffer *sb);
char *StringBufferToString(StringBuffer *sb, bool destroySb);
void StringBufferDestroy(StringBuffer *sb);

void str_realloc_cpy(char **dest, const char *src);
void strcut(char **str, const char *sub, const char *by);
void splitpprint(char **str);
void splitfree(char **str);
char **strsplit(const char *str, const char *delim);
char *str3dup(const char *str1, const char *str2, const char *str3);
char *str2dup(const char *str1, const char *str2);
char *str4dup(const char *str1, const char *str2, const char *str3, const char *str4);

char *strconcat(const char *strings[], int len);

#endif
