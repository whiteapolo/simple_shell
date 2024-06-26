#ifndef HISTORY_H
#define HISTORY_H

#include "config.h"
#include "data_structers/double_lll.h"
#include "data_structers/mystrings.h"


typedef struct {
	char *file_path;
	Sbuilder *buffer;
	int buff_index;
	Sbuilder *append_buffer;
} HistoryEntry;

void HistorySave(HistoryEntry *hist);
void HistoryFree(HistoryEntry *hist);
const char *HistoryFoward(HistoryEntry *hist);
const char *HistoryBackwars(HistoryEntry *hist);
void HistoryAppend(HistoryEntry *hist, const char *command);
void HistoryInit(HistoryEntry *hist, const char *file_path);


#endif
