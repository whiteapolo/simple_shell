#ifndef HISTORY_H
#define HISTORY_H

#include "config.h"


void HistoryInit();
void HistorySave();
void HistoryAppend(const char *command);


#endif
