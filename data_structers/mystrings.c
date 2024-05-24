#include "mystrings.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

Sbuilder *SbuilderCreate()
{
	Sbuilder *sb = malloc(sizeof(Sbuilder));
	sb->data = NULL;
	sb->size = 0;
	return sb;
}

void SbuilderAppend(Sbuilder *sb, const char *str)
{
	sb->data = realloc(sb->data, sizeof(char*) * ++sb->size);
	sb->data[sb->size - 1] = strdup(str);
}

void SbuilderRemove(Sbuilder *sb)
{
	sb->data = realloc(sb->data, sizeof(char*) * --sb->size);
}

void SbuilderAppendIndex(Sbuilder *sb, const char *str, int n)
{
	sb->data = realloc(sb->data, sizeof(char*) * (++sb->size));
	memmove(&sb->data[n+1], &sb->data[n], sizeof(char*) * (sb->size - n));
	sb->data[n] = strdup(str);
}

void SbuilderRemoveIndex(Sbuilder *sb, int n)
{
	free(sb->data[n]);
	memmove(&sb->data[n], &sb->data[n+1], sizeof(char*) * (sb->size - n));
	sb->data = realloc(sb->data, sizeof(char*) * --sb->size);
}

const char *SbuilderGet(Sbuilder *sb, int n)
{
	if (n < sb->size)
		return sb->data[n];
	return NULL;
}

/* end is marked with null pointer */
char **SbuilderToStrings(Sbuilder *sb, bool destroySb)
{
	char **s;
	if (destroySb) {
		sb->data = realloc(sb->data, sizeof(char*) * (sb->size + 1));
		sb->data[sb->size] = NULL;
		s = sb->data;
		free(sb);
	} else {
		s = malloc(sizeof(char*) * (sb->size + 1));
		for (int i = 0; i < sb->size; i++)
			s[i] = strdup(sb->data[i]);
		s[sb->size] = NULL;
	}

	return s;
}

char *SbuilderToString(Sbuilder *sb, const char *delim, bool destroySb)
{
	char *s = NULL;
	int size = 1;
	int delim_size = strlen(delim);
	int slen;


	for (int i = 0; i < sb->size; i++) {
		slen = strlen(sb->data[i]);
		size += slen + delim_size;
		s = realloc(s, sizeof(char)*size);
		strcat(s + size - (slen + delim_size), sb->data[i]);
		strcat(s + size - delim_size - 1, delim);
	}

	s[size - delim_size - 1] = '\0';

	if (destroySb)
		SbuilderDestroy(sb);

	return s;
}

void SbuilderDestroy(Sbuilder *sb)
{
	for (int i = 0; i < sb->size; i++)
		free(sb->data[i]);
	free(sb->data);
	free(sb);
}

Sbuilder *SbuilderCreateFromString(const char *str, const char *delim)
{
	char **split = strsplit(str, delim);
	Sbuilder *sb = SbuilderCreateFromStrings(split);
	splitfree(split);
	return sb;
}

Sbuilder *SbuilderCreateFromStrings(char **str)
{
	Sbuilder *sb = SbuilderCreate();

	if (str != NULL) {
		while (*str) {
			SbuilderAppend(sb, *str);
			str++;
		}
	}

	return sb;
}

Sbuilder *SbuilderDup(Sbuilder *sb)
{
	char **str = SbuilderToStrings(sb, false);
	return SbuilderCreateFromStrings(str);
}

/* concat sb2 to sb1, sb2 is unharmed */
void SbuilderConcat(Sbuilder *sb1, Sbuilder *sb2)
{
	char **str = SbuilderToStrings(sb2, false);
	if (str == NULL)
		return;
	while (*str) {
		SbuilderAppend(sb1, *str);
		str++;
	}
}

void splitpprint(char **str)
{
	while (*str)
		printf("%s\n", *(str++));
}

void splitfree(char **str)
{
	if (str == NULL)
		return;

	int i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

void strncut(char *str, const char *sub, const char *by, int n)
{
	const char *start = strstr(str, sub);
	if (start == NULL)
		return;

	char tmp[n];
	strcpy(tmp, str);

	tmp[start - str] = '\0';
	strcat(tmp, by);
	strcat(tmp, start + strlen(sub));
	strcpy(str, tmp);
}

void str_realloc_cpy(char **dest, const char *src)
{
	*dest = realloc(*dest, sizeof(char) * (strlen(src) + 1));
	strcpy(*dest, src);
}

void strcut(char **str, const char *sub, const char *by)
{
	if (!strcmp(*str, sub)) {
		*str = realloc(*str, sizeof(char) * strlen(by));
		strcpy(*str, by);
		return;
	}

	char *str_start = *str;
	char *start = strstr(*str, sub);
	if (start == NULL)
		return;

	char *end = start + strlen(sub);

	int size = strlen(*str) - strlen(sub) + strlen(by) + 1;
	char *dest = malloc(sizeof(char) * size);
	char *dest_start = dest;

	while (*str != start)
		*(dest++) = *((*str)++);

	while (*by)
		*(dest++) = *(by++);

	*str = end;

	while (**str)
		*(dest++) = *((*str)++);

	*dest = '\0';

	*str = str_start;
	free(*str);
	*str = dest_start;
}

Sbuffer *SbufferCreate()
{
	Sbuffer *sb = malloc(sizeof(Sbuffer));
	sb->data = NULL;
	sb->size = 0;
	return sb;
}

void SbufferAppend(Sbuffer *sb, char c)
{
	sb->data = realloc(sb->data, sizeof(char) * ++sb->size);
	sb->data[sb->size - 1] = c;
}

char SbufferRemove(Sbuffer *sb)
{
	if (sb->size == 0)
		return 0;

	char c = sb->data[sb->size - 1];
	sb->data = realloc(sb->data, sizeof(char) * --sb->size);
	return c;
}

char *SbufferToString(Sbuffer *sb, bool destroySb)
{
	char *s;
	SbufferAppend(sb, '\0');
	if (destroySb) {
		s = sb->data;
		free(sb);
	} else {
		s = strdup(sb->data);	
	}
	return s;
}

void SbufferDestroy(Sbuffer *sb)
{
	free(sb->data);
	free(sb);
}

char **strsplit(const char *str, const char *delim)
{
	int i = 0;
	char *cpy =  strdup(str);
	char *ptr = strtok(cpy, delim);
	char **split = malloc(sizeof(char*));
	while (ptr) {
		split = realloc(split, sizeof(char*) * (i+2));
		split[i++] = strdup(ptr);
		ptr = strtok(NULL, delim);
	}
	split[i] = NULL;
	free(cpy);
	return split;
}

void strdevide(const char *str, int index, char **s1, char **s2)
{
	*s1 = strndup(str, index + 1);
	*s2 = strdup(str + index + 1);
}

void strpush(char **str, char c)
{
	int len = strlen(*str);
	*str = realloc(*str, sizeof(char) * (len + 2));
	(*str)[len] = c;
	(*str)[len + 1] = '\0';
}

void strpush_string(char **str, const char *s)
{
	int l1 = strlen(*str);
	int l2 = strlen(s);
	*str = realloc(*str, sizeof(char) * (l1 + l2 + 1));
	strcpy(*str + l1, s);
}

char strpop(char **str)
{
	int len = strlen(*str);
	*str = realloc(*str, sizeof(char) * len);
	char c = (*str)[len - 1];
	(*str)[len - 1] = '\0';
	return c;
}

char *strconcat(const char *strings[], int len)
{
	int l = 1;
	int larr[len];
	for (int i = 0; i < len; i++) {
		larr[i] = strlen(strings[i]);
		l += larr[i];
	}

	char *str = malloc(sizeof(char) * l);
	char *ptr = str;
	for (int i = 0; i < len; i++) {
		strcpy(ptr, strings[i]);
		ptr += larr[i];
	}
	return str;
}

void splitconcat(char ***dest, char **src)
{
	Sbuilder *sb1 = SbuilderCreateFromStrings(*dest);
	Sbuilder *sb2 = SbuilderCreateFromStrings(src);
	SbuilderConcat(sb1, sb2);
	splitfree(*dest);
	*dest = SbuilderToStrings(sb1, true);
}

char *str4dup(const char *str1, const char *str2, const char *str3, const char *str4)
{
	int l1 = strlen(str1);
	int l2 = strlen(str2);
	int l3 = strlen(str3);
	int l4 = strlen(str4);
	char *str = malloc(sizeof(char) * (l1 + l2 + l3 + l4 + 1));
	strcpy(str, str1);
	char *tmp = str + l1;
	strcpy(tmp, str2);
	tmp += l2;
	strcpy(tmp, str3);
	tmp += l3;
	strcpy(tmp, str4);
	return str;
}

int str_prefix_len(const char *s1, const char *s2)
{
	int n = 0;
	while (*s1 && *s2 && *(s1++) == *(s2++)) n++;
	return n;
}

char *common_prefix(char **str, int len)
{
	if (str == NULL)
		return strdup("");

	if (len == 1)
		return strdup(*str);

	int length = str_prefix_len(str[0], str[1]);

	const char *base = str[0];
	str += 2;

	while (length != 0 && *str != NULL) {
		length = MIN(length, str_prefix_len(base, *str));
		str++;
	}

	return strndup(base, length);
}

char *str3dup(const char *str1, const char *str2, const char *str3)
{
	int l1 = strlen(str1);
	int l2 = strlen(str2);
	int l3 = strlen(str3);
	char *str = malloc(sizeof(char) * (l1 + l2 + l3 + 1));
	strcpy(str, str1);
	char *tmp = str + l1;
	strcpy(tmp, str2);
	tmp += l2;
	strcpy(tmp, str3);
	return str;
}

char *str2dup(const char *str1, const char *str2)
{
	int l1 = strlen(str1);
	int l2 = strlen(str2);
	char *str = malloc(sizeof(char) * (l1 + l2 + 1));
	strcpy(str, str1);
	char *tmp = str + l1;
	strcpy(tmp, str2);
	return str;
}
