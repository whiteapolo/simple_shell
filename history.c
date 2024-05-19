#include "history.h"
#include "config.h"
#include "data_structers/double_lll.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static dlist_t *hist_lst;
static dlist_t *hist_ptr;
static dlist_t *hist_end;

void loadHistoryFile()
{
	dlist_init(&hist_lst);
	dlist_push(&hist_lst, NULL);
	hist_ptr = hist_lst;
	FILE *fp = fopen(history_file, "a+");
	if (fp == NULL) {
		printf("Cannot open history file at: \"%s\"\n", history_file);
		exit(1);
	}
	fseek(fp, 0, SEEK_SET);
	char buff[1024];
	while (fgets(buff, 1024, fp)) {
		buff[strlen(buff) - 1] = '\0';
		dlist_insert_after(hist_ptr, strdup(buff));
		hist_ptr = hist_lst->next;
	}

	fclose(fp);
}

void HistoryInit()
{
	loadHistoryFile();
}

void HistoryAppend(const char *command)
{
	dlist_insert_after(hist_ptr, strdup(command));
	hist_ptr = hist_lst->next;
}

void HistorySave()
{
	FILE *fp = fopen(history_file, "w");
	if (fp == NULL) {
		printf("Cannot open history file at: \"%s\"\n", history_file);
		exit(1);
	}
	dlist_t *cnt = hist_lst;

	while ((cnt = cnt->next)) {
		fwrite(cnt->info, sizeof(char), strlen(cnt->info), fp);
		fputc('\n', fp);
	}

	fclose(fp);
}
