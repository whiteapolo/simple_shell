#ifndef CMD_H
#define CMD_H

#include "data_structers/string_builder.h"
#include "data_structers/dict.h"
#include "types.h"

#define FILE_EXISTS(p) (!access(p, F_OK))

typedef StringBuilder Cmd;

Cmd *CmdCreate();
void CmdAppend(Cmd *cmd, const char *arg);
void CmdDestroy(Cmd *cmd);
int CmdRun(Cmd *cmd, Dict *programs_dict, Dict *alias_dict, bool destroy);
void CmdReplace(Cmd *cmd, Cmd *src, u16 n);
const char *CmdGet(Cmd *cmd, u16 n);
Cmd *CmdCreateFromString(const char *str);


#endif
