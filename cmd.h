#ifndef CMD_H
#define CMD_H

#include "data_structers/mystrings.h"
#include "data_structers/dict.h"
#include "types.h"

#define FILE_EXISTS(p) (!access(p, F_OK))
#define IS_EMPTY_COMMAND(str) (IS_EMPTY_STRING(str) || strpbrk(str, " \t\n\v\f\r") == str)

typedef Sbuilder Cmd;

Cmd *CmdCreate();
void CmdAppend(Cmd *cmd, const char *arg);
void CmdDestroy(Cmd *cmd);
int CmdRun(Cmd *cmd, Dict *programs_dict, Dict *alias_dict, bool destroy);
void CmdReplace(Cmd *cmd, Cmd *src, u16 n);
const char *CmdGet(Cmd *cmd, u16 n);
Cmd *CmdCreateFromString(const char *str);


#endif
