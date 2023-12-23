#ifndef MISCTOOLS_H
#define MISCTOOLS_H

#include "../Errors/I_Errors.h"

enum TP_ERROR_TYPES AppendToArrayOfPointers(void ***targetArray, size_t *targetArrayLength, void *ToAppend, size_t elementSize);
enum TP_ERROR_TYPES FreeArrayOfPointers(void ***targetArray, size_t targetArrayLength);

char* TP_GetIntRangeStr(int _offset, int _val);

TPTable_Conf *ReadTableConfig(TPTable *_self);

char **TP_GetFileNamesInDir(char *_path, size_t _count);

#endif