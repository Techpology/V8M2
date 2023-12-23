#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <dirent.h>

#include "MiscTools.h"
#include "StringTools.h"
#include "../Storage/Storage.h"

enum TP_ERROR_TYPES AppendToArrayOfPointers(void ***targetArray, size_t *targetArrayLength, void *ToAppend, size_t elementSize)
{
	void **RetArray = (void**)realloc(*targetArray, elementSize * ((*targetArrayLength) + 1));

	if (RetArray == NULL) {
		if (ENABLE_ERROR_LOG) {
			printf("%s\n", ERROR_ASCII_FAIL);
		}
		return TP_FAILED_AppendToArrayOfPointers;
	}

	RetArray[*targetArrayLength] = malloc(elementSize);
	if (RetArray[*targetArrayLength] == NULL) {
		free(RetArray);
		if (ENABLE_ERROR_LOG) {
			printf("%s\n", ERROR_ASCII_FAIL);
		}
		return TP_FAILED_AppendToArrayOfPointers;
	}

	memcpy(RetArray[*targetArrayLength], ToAppend, elementSize);

	(*targetArrayLength)++;
	*targetArray = RetArray;

	if (ENABLE_ERROR_LOG) {
		printf("%s:", ERROR_ASCII_SUCCESS);
	}
	return TP_SUCCESS;
}

enum TP_ERROR_TYPES FreeArrayOfPointers(void ***targetArray, size_t targetArrayLength)
{
	if((*targetArray) != NULL)
	{
		for (size_t i = 0; i < targetArrayLength; i++)
		{
			if((*targetArray)[i] != NULL)
			{
				free((*targetArray)[i]);
				(*targetArray)[i] = NULL;
			}
			else
			{
				return TP_FAILED_FreeArrayOfPointers;
			}
		}
		free((*targetArray));
		(*targetArray) = NULL;
		return TP_SUCCESS;
	}
	else
	{
		return TP_FAILED_FreeArrayOfPointers;
	}
}

char* TP_GetIntRangeStr(int _offset, int _val)
{
	int i = floor(_val/_offset) * _offset;
	int j = i + _offset;
	
	size_t retSize = snprintf(NULL, 0, "%d-%d", i, j) + 1;
	char* ret = (char*)malloc(sizeof(char) * retSize);
	sprintf(ret, "%d-%d", i, j);

	return ret;
}

TPTable_Conf *ReadTableConfig(TPTable *_self)
{
	TPTable_Conf *ToRet = (TPTable_Conf*)malloc(sizeof(TPTable_Conf));
	
	char *ConfPath = TP_StrnCat(_self->ParentDatabase->ConfigPath, 2, _self->Name, "/Conf.txt");
	char *ConfText = TP_ReadFile(ConfPath);

	int ConfLinesCount = 0;
	char **ConfLines = TP_SplitString(ConfText, '\n', &ConfLinesCount);

	for (int i = 0; i < ConfLinesCount; i++)
	{
		char **ConfLineSplit = TP_SplitString(ConfLines[i], ':', NULL);

		switch (i)
		{
			case 0:
				ToRet->LastId = atoi(ConfLineSplit[1]);
				break;
			case 1:
				ToRet->RowCount = atoi(ConfLineSplit[1]);
				break;
			case 2:
				ToRet->LazyLoad = atoi(ConfLineSplit[1]);
				break;
		}

		free(ConfLineSplit[1]); ConfLineSplit[1] = NULL;
		free(ConfLineSplit[0]); ConfLineSplit[0] = NULL;
		free(ConfLineSplit); ConfLineSplit = NULL;
	}

	FreeArrayOfPointers((void***)&ConfLines, ConfLinesCount);
	free(ConfPath); ConfPath = NULL;
	free(ConfText); ConfText = NULL;

	return ToRet;
}

char **TP_GetFileNamesInDir(char *_path, size_t _count)
{
	DIR *dir;
	struct dirent *ent;

	// if(_count == NULL)
	// {
		// dynamic array
		// Don't need it right now and I have to work fast, wont implement yet.
	// }

	char **toRet = (char**)malloc(sizeof(char*) * _count);
	int i = 0;

	dir = opendir(_path);
	if(dir != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			toRet[i] = TP_StrnCat(_path, 2, "/", ent->d_name);
			i++;
		}
		closedir(dir);
	}

	return toRet;
}
