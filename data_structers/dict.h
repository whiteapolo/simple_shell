/*
 * dictionary for the config file
 * implemented by open hash table to a linear linked list
 */

#ifndef DICT_H
#define DICT_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lll.h"


typedef struct {
	char *key;
	void *info;
} Val;

typedef struct {
	lll_t **managers;
	unsigned int size;
} Dict;

unsigned long hash(const char *str);

void dict_init(Dict *dict, int size);
void dict_insert(Dict *dict, char *key, void *info);
void dict_remove(Dict *dict, char *key);
void *dict_find(Dict *dict, char *key);
void dict_destroy(Dict *dict);

#endif
