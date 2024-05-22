#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"
#include "config.h"
#include "data_structers/double_lll.h"
#include "data_structers/mystrings.h"
#include "types.h"

static const char *empty_string = "";

void loadHistory(HistoryEntry *hist)
{
	dlist_init(&hist->lst);
	dlist_push(&hist->lst, NULL);
	hist->end = hist->lst;
	FILE *fp = fopen(hist->file_path, "a+");
	if (fp == NULL) {
		printf("Cannot open history file at: \"%s\"\n", hist->file_path);
		exit(1);
	}
	fseek(fp, 0, SEEK_SET);
	char buff[1024];
	while (fgets(buff, 1024, fp)) {
		buff[strlen(buff) - 1] = '\0';
		dlist_insert_after(hist->end, strdup(buff));
		hist->end = hist->end->next;
	}

	dlist_insert_after(hist->end, NULL);

	hist->ptr = hist->end->next;

	fclose(fp);
}

void HistoryInit(HistoryEntry *hist, const char *file_path)
{
	hist->file_path = strdup(file_path);
	loadHistory(hist);
	hist->command_buffer = SbuilderCreate();
}

void HistoryAppend(HistoryEntry *hist, const char *command)
{
	dlist_insert_after(hist->end, strdup(command));
	hist->end = hist->end->next;
	hist->ptr = hist->end->next;
	SbuilderAppend(hist->command_buffer, command);
}

const char *HistoryBackwars(HistoryEntry *hist)
{
	if (hist->ptr->pre->info == NULL)
		return NULL;
	hist->ptr = hist->ptr->pre;
	return hist->ptr->info;
}

const char *HistoryFoward(HistoryEntry *hist)
{
	if (hist->ptr->next == NULL) {
		return empty_string;
	} else if (hist->ptr->next->info == NULL) {
		hist->ptr = hist->ptr->next;
		return empty_string;
	}

	hist->ptr = hist->ptr->next;
	return hist->ptr->info;
}

void HistoryFree(HistoryEntry *hist)
{
	if (hist->command_buffer != NULL)
		SbuilderDestroy(hist->command_buffer);
}

void HistorySave(HistoryEntry *hist)
{
	if (hist->command_buffer == NULL || hist->command_buffer->size == 0)
		return;

	FILE *fp = fopen(hist->file_path, "a");
	if (fp == NULL) {
		printf("Cannot open history file at: \"%s\"\n", hist->file_path);
		exit(1);
	}

	char **commands = SbuilderToStrings(hist->command_buffer, true);
	hist->command_buffer = SbuilderCreate();

	int i = 0;
	while (commands[i]) {
		fwrite(commands[i], sizeof(char), strlen(commands[i]), fp);
		fputc('\n', fp);
		i++;
	}
	splitfree(commands);

	fclose(fp);
}
