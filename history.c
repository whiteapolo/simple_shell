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
	SbuilderAppend(hist->buffer, empty_string);
	FILE *fp = fopen(hist->file_path, "a+");
	if (fp == NULL) {
		printf("Cannot open history file at: \"%s\"\n", hist->file_path);
		return;
	}
	fseek(fp, 0, SEEK_SET);
	char buff[1024];
	while (fgets(buff, 1024, fp)) {
		buff[strlen(buff) - 1] = '\0';
		SbuilderAppend(hist->buffer, buff);
	}

	hist->buff_index = hist->buffer->size - 1;

	fclose(fp);
}

void HistoryInit(HistoryEntry *hist, const char *file_path)
{
	hist->file_path = strdup(file_path);
	hist->buffer = SbuilderCreate();
	hist->append_buffer = SbuilderCreate();
	hist->buff_index = -1;
	loadHistory(hist);
}

void HistoryAppend(HistoryEntry *hist, const char *command)
{
	SbuilderAppend(hist->append_buffer, command);
	SbuilderAppend(hist->buffer, command);
	hist->buff_index = hist->buffer->size - 1;
}

const char *HistoryBackwars(HistoryEntry *hist)
{
	if (!IN_BOUNDRY(hist->buff_index, 1, hist->buffer->size - 1))
		return empty_string;
	return hist->buffer->data[hist->buff_index--];
}

const char *HistoryFoward(HistoryEntry *hist)
{
	if (!IN_BOUNDRY(hist->buff_index, hist->buff_index, hist->buffer->size - 2))
		return empty_string;
	return hist->buffer->data[++hist->buff_index];
}

void HistoryFree(HistoryEntry *hist)
{
	SbuilderDestroy(hist->buffer);
	SbuilderDestroy(hist->append_buffer);
	free(hist->file_path);
}

void HistorySave(HistoryEntry *hist)
{
	if (hist->append_buffer == NULL || hist->append_buffer->size == 0)
		return;

	FILE *fp = fopen(hist->file_path, "a");
	if (fp == NULL) {
		return;
	}

	char **commands = SbuilderToStrings(hist->append_buffer, true);
	hist->append_buffer = SbuilderCreate();

	int i = 0;
	while (commands[i]) {
		fwrite(commands[i], sizeof(char), strlen(commands[i]), fp);
		fputc('\n', fp);
		i++;
	}
	splitfree(commands);

	fclose(fp);
}
