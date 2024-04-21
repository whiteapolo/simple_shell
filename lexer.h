#ifndef LEXER_H
#define LEXER_H

#include <string.h>
#include "data_structers/lll.h"
#include "strings.h"

static lll_t *tokinize(const char *str);
static void untokinize(lll_t *start, lll_t *end, char **split);

typedef enum {
	TYPE_COMMAND,
	TYPE_SEMICOLON,
	TYPE_AMPERCENT,
	TYPE_DOUBLE_AMPERCENT,
	TYPE_PIPE,
} TOKEN_TYPE;

typedef struct {
	TOKEN_TYPE type;
	char *info;
} Token;


static void *mk_token(const char *info, TOKEN_TYPE type)
{
	Token *t = malloc(sizeof(Token));
	if (info)
		t->info = strdup(info);
	else
		t->info = NULL;
	t->type = type;
	return t;
}

/* including start, not including end */
static void untokinize(lll_t *start, lll_t *end, char **split)
{
	while (start != end)
		*(split++) = ((Token*)start->info)->info;
}

static Token *get_token(lll_t *l)
{
	return l->info;
}

static lll_t *tokinize2(const char *str)
{

}

static lll_t *tokinize(const char *str)
{
	lll_t *start;
	lll_t *cnt;
	lll_init(&start);
	lll_push(&start, NULL);
	cnt = start;
	char buff[256] = {0};
	char i = 0;

	while (*str) {
		switch (*str) {
		case ';':
			if (i > 0) {
				buff[i] = '\0';
				lll_insert_after(cnt, mk_token(buff, TYPE_COMMAND));
				cnt = cnt->next;
				i = 0;
			}
			lll_insert_after(cnt, mk_token(NULL, TYPE_SEMICOLON));
			cnt = cnt->next;
			break;
		case '&':
			if (i > 0) {
				buff[i] = '\0';
				lll_insert_after(cnt, mk_token(buff, TYPE_COMMAND));
				cnt = cnt->next;
				i = 0;
			}
			if (str[1] == '&') {
				lll_insert_after(cnt, mk_token(NULL, TYPE_DOUBLE_AMPERCENT));
				str++;
			} else {
				lll_insert_after(cnt, mk_token(NULL, TYPE_AMPERCENT));
			}
			cnt = cnt->next;
			break;
		case '|':
			if (i > 0) {
				buff[i] = '\0';
				lll_insert_after(cnt, mk_token(buff, TYPE_COMMAND));
				cnt = cnt->next;
				i = 0;
			}
			lll_insert_after(cnt, mk_token(NULL, TYPE_PIPE));
			cnt = cnt->next;
			break;
		case ' ':
			if (i > 0) {
				buff[i] = '\0';
				lll_insert_after(cnt, mk_token(buff, TYPE_COMMAND));
				cnt = cnt->next;
				i = 0;
			}
			break;
		default:
			buff[i++] = *str;
		}

		str++;
	}
	if (i > 0) {
		buff[i] = '\0';
		lll_insert_after(cnt, mk_token(buff, TYPE_COMMAND));
		cnt = cnt->next;
		i = 0;
	}
	lll_pop(&start);
	return start;
}

#endif
