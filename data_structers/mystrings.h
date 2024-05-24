#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdbool.h>
#include <string.h>

typedef struct {
	char **data;
	int size;
} Sbuilder;

typedef struct {
	char *data;
	int size;
} Sbuffer;

#define IS_EMPTY_STRING(str) (!(str && (*str)))
#define CHAR_TO_STR(c) ((char[]){c, '\0'})
#define EMPTY_SPLIT() (SbuilderToStrings(SbuilderCreate(), true))

Sbuilder *SbuilderCreate();
void SbuilderAppend(Sbuilder *sb, const char *str);
void SbuilderAppendIndex(Sbuilder *sb, const char *str, int n);
void SbuilderRemoveIndex(Sbuilder *sb, int n);
const char *SbuilderGet(Sbuilder *sb, int n);
char **SbuilderToStrings(Sbuilder *sb, bool destroySb);
char *SbuilderToString(Sbuilder *sb, const char *delim, bool destroySb);
void SbuilderDestroy(Sbuilder *sb);
void SbuilderConcat(Sbuilder *sb1, Sbuilder *sb2);
Sbuilder *SbuilderDup(Sbuilder *sb);
Sbuilder *SbuilderCreateFromStrings(char **str);
Sbuilder *SbuilderCreateFromString(const char *str, const char *delim);

Sbuffer *SbufferCreate();
void SbufferAppend(Sbuffer *sb, char c);
char SbufferRemove(Sbuffer *sb);
char *SbufferToString(Sbuffer *sb, bool destroySb);
void SbufferDestroy(Sbuffer *sb);

void splitconcat(char ***dest, char **src);
void str_realloc_cpy(char **dest, const char *src);
void strcut(char **str, const char *sub, const char *by);
void splitpprint(char **str);
void splitfree(char **str);
char **strsplit(const char *str, const char *delim);
char *str3dup(const char *str1, const char *str2, const char *str3);
char *str2dup(const char *str1, const char *str2);
char *str4dup(const char *str1, const char *str2, const char *str3, const char *str4);
void strdevide(const char *str, int index, char **s1, char **s2);
void strpush(char **str, char c);
char strpop(char **str);
void strpush_string(char **str, const char *s);

int str_prefix_len(const char *s1, const char *s2);
char *common_prefix(char **str, int len);
char *strconcat(const char *strings[], int len);

#endif
