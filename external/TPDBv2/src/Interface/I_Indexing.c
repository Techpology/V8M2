#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../Errors/I_Errors.h"
#include "../Storage/Storage.h"
#include "../Utils/StringTools.h"
#include "../Utils/MiscTools.h"

enum TP_ERROR_TYPES TP_InsertRowToIndexTable(TPTable *_Table, TPTable_Row *_Row, char *_RangeStr, int _ColIndex)
{
	size_t IndexTablePathSize = snprintf(NULL, 0, "%s%s/%d.cif", _Table->ParentDatabase->ConfigPath, _Table->Name, _ColIndex) + 1;
	char * IndexTablePath = (char*)malloc(sizeof(char) * IndexTablePathSize);
	sprintf(IndexTablePath, "%s%s/%d.cif", _Table->ParentDatabase->ConfigPath, _Table->Name, _ColIndex);

	char *IndexTableStr = strdup("");
	if(access(IndexTablePath, F_OK) == 0)
	{
		free(IndexTableStr);
		IndexTableStr = TP_ReadFile(IndexTablePath);
	}

	int IndexTableLinesCount = 0;
	char **IndexTableLines = NULL;
	int TargetLineInIndex = -1;
	char *Vals = "";
	if(strcmp(IndexTableStr, "") != 0)
	{
		IndexTableLines = TP_SplitString(IndexTableStr, '\n', &IndexTableLinesCount);
		
		if(IndexTableLines == NULL)
		{
			IndexTableLines = (char**)malloc(sizeof(char*));
			IndexTableLines[0] = strdup(IndexTableStr);
			IndexTableLinesCount = 1;
		}

		char *RangeStr_Tail = TP_StrnCat(_RangeStr, 1, ":");
		for (int i = 0; i < IndexTableLinesCount; i++)
		{
			if(strstr(IndexTableLines[i], RangeStr_Tail) != NULL)
			{
				TargetLineInIndex = i;
				int TempKeyValCount = 0;
				char **TempKeyVal = TP_SplitString(IndexTableLines[i], ':', &TempKeyValCount);
				Vals = strdup(TempKeyVal[1]);

				free(TempKeyVal[1]); TempKeyVal[1] = NULL;
				free(TempKeyVal[0]); TempKeyVal[0] = NULL;
				free(TempKeyVal); TempKeyVal = NULL;
				break;
			}
		}
		free(RangeStr_Tail); RangeStr_Tail = NULL;
	}

	size_t RowIdSize = snprintf(NULL, 0, "%d", _Row->_ID) + 1;
	char *RowIdStr = (char*)malloc(sizeof(char) * RowIdSize);
	sprintf(RowIdStr, "%d", _Row->_ID);

	char *ValsComplete = "";
	if(TargetLineInIndex == -1)
	{
		ValsComplete = RowIdStr;
	}
	else
	{
		ValsComplete = TP_StrnCat(Vals, 2, ",", RowIdStr);
	}

	char *ToInsert = TP_StrnCat(_RangeStr, 2, ":", ValsComplete);
	if(TargetLineInIndex != -1)
	{
		free(IndexTableLines[TargetLineInIndex]);
		IndexTableLines[TargetLineInIndex] = ToInsert;
	}
	else
	{
		if(IndexTableLines == NULL)
		{
			IndexTableLines = malloc(sizeof(char*) * 1);
		}
		else
		{
			IndexTableLines = realloc(IndexTableLines, sizeof(char*) * (IndexTableLinesCount + 1));
		}
		IndexTableLines[IndexTableLinesCount] = ToInsert;
		IndexTableLinesCount++;
	}

	char *NewIndexTableFile;
	if(IndexTableLinesCount <= 1)
	{
		NewIndexTableFile = strdup(ToInsert);
	}
	else
	{
		NewIndexTableFile = TP_StrnCatArray(IndexTableLines, IndexTableLinesCount, "\n");
	}
	TP_CheckError(TP_StoreFile(IndexTablePath, NewIndexTableFile), TP_EXIT);

	if(IndexTableLines != NULL)
	{
		for (int i = 0; i < IndexTableLinesCount; i++)
		{
			if(IndexTableLines[i] != NULL){ free(IndexTableLines[i]); IndexTableLines[i] = NULL; }
		}
		free(IndexTableLines);
		IndexTableLines = NULL;
	}

	if(ValsComplete != NULL){ free(ValsComplete); ValsComplete = NULL; }
	if(Vals != NULL && strcmp(Vals, "") != 0){ free(Vals); Vals = NULL; }
	
	if(TargetLineInIndex != -1 && RowIdStr != NULL){ free(RowIdStr); RowIdStr = NULL; }
	
	if(NewIndexTableFile != NULL){ free(NewIndexTableFile); NewIndexTableFile = NULL; }

	if(IndexTablePath != NULL){ free(IndexTablePath); IndexTablePath = NULL; }
	if(IndexTableStr != NULL){ free(IndexTableStr); IndexTableStr = NULL; }
	
	return TP_SUCCESS;
}

int *TP_GetIndexAtRange(TPTable *_Table, int _col, int _val, int *ResultCount)
{
	size_t IndexTablePathSize = snprintf(NULL, 0, "%s%s/%d.cif", _Table->ParentDatabase->ConfigPath, _Table->Name, _col) + 1;
	char * IndexTablePath = (char*)malloc(sizeof(char) * IndexTablePathSize);
	sprintf(IndexTablePath, "%s%s/%d.cif", _Table->ParentDatabase->ConfigPath, _Table->Name, _col);
	puts(IndexTablePath);

	char *IndexTableStr = TP_ReadFile(IndexTablePath);
	free(IndexTablePath); IndexTablePath = NULL;
	puts(IndexTableStr);

	if(strlen(IndexTableStr) <= 0) { free(IndexTableStr); IndexTableStr=NULL; return NULL; }

	int IndexTableLinesCount = 0;
	puts("PreHERE");
	char **IndexTableLines = TP_SplitString(IndexTableStr, '\n', &IndexTableLinesCount);

	if(IndexTableLinesCount <= 1 || IndexTableLines == NULL)
	{
		IndexTableLinesCount = 1;
		if(IndexTableLines != NULL){ FreeArrayOfPointers((void***)&IndexTableLines, 1); }
		IndexTableLines = (char**)malloc(sizeof(char*));
		IndexTableLines[0] = IndexTableStr;
	}
	printf("HERE: %s\n", IndexTableLines[0]);
	free(IndexTableStr); IndexTableStr = NULL;

	char *ValRangeStr = TP_GetIntRangeStr(_Table->ColumnsIndexOffset, _val);
	printf("ValRangeStr: %s\n", ValRangeStr);
	char *ValRangeStr_Tail = TP_StrnCat(ValRangeStr, 1, ":");
	printf("ValRangeStr_Tail: %s\n", ValRangeStr_Tail);
	int TargetLine = -1;

	for (int i = 0; i < IndexTableLinesCount; i++)
	{
		if(strstr(IndexTableLines[i], ValRangeStr_Tail) != NULL)
		{
			TargetLine = i;
			break;
		}
	}
	if(ValRangeStr != NULL) { free(ValRangeStr); ValRangeStr = NULL; }
	if(ValRangeStr_Tail != NULL) { free(ValRangeStr_Tail); ValRangeStr_Tail = NULL; }
	if(TargetLine == -1)
	{
		puts("!!!-[-1]-!!!");
		if(IndexTableLines != NULL) { FreeArrayOfPointers((void***)&IndexTableLines, IndexTableLinesCount); }
		return NULL;
	}
	printf("TargetLine: %d\n", TargetLine);

	char **IndexLineKeyVal = TP_SplitString(IndexTableLines[TargetLine], ':', NULL);
	printf("key: %s, val:%s\n", IndexLineKeyVal[0], IndexLineKeyVal[1]);
	char *IndexVal = strdup(IndexLineKeyVal[1]);

	FreeArrayOfPointers((void***)&IndexLineKeyVal, 2);
	if(IndexTableLines != NULL) { FreeArrayOfPointers((void***)&IndexTableLines, IndexTableLinesCount); }

	int ValCount = 0;
	char **Vals = TP_SplitString(IndexVal, ',', &ValCount);
	int *toRet = (int*)malloc(sizeof(int) * ValCount);

	for (int i = 0; i < ValCount; i++)
	{
		toRet[i] = atoi(Vals[i]);
	}
	if(ResultCount != NULL){ (*ResultCount) = ValCount; }

	free(IndexVal); IndexVal = NULL;
	FreeArrayOfPointers((void***)&Vals, ValCount);
	return toRet;
}
