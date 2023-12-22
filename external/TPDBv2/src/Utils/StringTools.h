#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

char *TP_StrnCat(char *_baseString, int _count, ...);

char *TP_StrnCatArray(char **_list, int _count, char *_separator);

char **TP_SplitString(char *_str, char _separator, int *_returnCount);

#endif
