#ifndef ALIAS_H
#define ALIAS_H

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "data_structers/dict.h"
#include "data_structers/lll.h"
#include "lexer.h"

static Dict alias_dict;

void alias_init()
{
	dict_init(&alias_dict, ARRAY_SIZE(aliases));
	FOR(i, ARRAY_SIZE(aliases)) {
		dict_insert(&alias_dict, aliases[i][0], tokinize(aliases[i][1]));
	}
}

void print_info(void *info)
{
	printf("INFO: %s TYPE: %d\n", ((Token*)info)->info, ((Token*)info)->type);
}

void alias_proccess(lll_t **token_list)
{

	lll_t *alias;
	/* puts("before alias:"); */
	/* puts("--------------"); */
	/* lll_print(*token_list, print_info); */

	if (token_list) {
		alias = dict_find(&alias_dict, ((Token*)((*token_list)->info))->info);
		if (alias) {
			free(lll_pop(token_list));
			alias->next = *token_list;
			*token_list = alias;
		}
	}

	lll_t *before = *token_list;
	lll_t *cnt = (*token_list)->next;

	while (cnt) {
		if (get_token(cnt)->info) {
			alias = dict_find(&alias_dict, get_token(cnt)->info);
			if (alias) {
				lll_get_last(alias)->next = cnt->next;
				free(lll_remove_after(before));
				cnt = alias;
				before->next = cnt;
			}
		}
		before = cnt;
		cnt = cnt->next;
	}
	/* puts("\nafter alias:"); */
	/* puts("--------------"); */
	/* lll_print(*token_list, print_info); */
	/* puts("\n"); */
}

#endif
