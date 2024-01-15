#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Errors/I_Errors.h"
#include "../Storage/Storage.h"
#include "../Utils/StringTools.h"
#include "../Utils/MiscTools.h"
#include "I_Database.h"
#include "I_Table.h"
#include "I_Row.h"

TPDatabase *CreateTPDatabase(char *_Name, char *_Path)
{
	TPDatabase *newTPDB = (TPDatabase*)malloc(sizeof(TPDatabase));
	newTPDB->_ID = 0;
	newTPDB->Name = strdup(_Name);
	newTPDB->Path = strdup(_Path);
	
	newTPDB->ConfigPath = TP_StrnCat(_Path, 1, "/Config");
	newTPDB->TablesCount = 0;
	newTPDB->Tables = NULL;

	TP_CheckError(TP_Mkdir(newTPDB->Path), TP_IGNORE);
	TP_CheckError(TP_Mkdir(newTPDB->ConfigPath), TP_IGNORE);
	return newTPDB;
}

void DestroyTPDatabase(TPDatabase *_self)
{
	if(_self != NULL)
	{
		if(_self->Tables != NULL)
		{
			for (int i = 0; i < _self->TablesCount; i++)
			{
				DestroyTPTable(_self->Tables[i]);
			}
			free(_self->Tables);
		}
		if(_self->Name != NULL)
		{
			free(_self->Name);
			_self->Name = NULL;
		}
		if(_self->Path != NULL)
		{
			free(_self->Path);
			_self->Path = NULL;
		}
		if(_self->ConfigPath != NULL)
		{
			free(_self->ConfigPath);
			_self->ConfigPath = NULL;
		}
		free(_self);
	}
}

enum TP_ERROR_TYPES AddTable(TPDatabase *_self, char *_Name)
{
	char *_Name_TailSlash = TP_StrnCat("/", 1, _Name);
	TPTable *newTbl = CreateTPTable(_Name_TailSlash, _self, TP_FALSE);
	free(_Name_TailSlash);

	size_t TableConfigFolderPathSize = snprintf(NULL, 0, "%s%s", _self->ConfigPath, newTbl->Name) + 1;
	char * TableConfigFolderPath = (char*)malloc(sizeof(char) * TableConfigFolderPathSize);
	sprintf(TableConfigFolderPath, "%s%s", _self->ConfigPath, newTbl->Name);
	TP_Mkdir(TableConfigFolderPath);

	char *TableConfFilePath = TP_StrnCat(TableConfigFolderPath, 1, "/Conf.txt");
	TP_CheckError(TP_StoreFile(TableConfFilePath, "LastId:0\nRowCount:0\nLazyLoad:1"), TP_EXIT);

	free(TableConfigFolderPath); TableConfigFolderPath = NULL;
	free(TableConfFilePath); TableConfFilePath = NULL;

	//AppendToArrayOfPointers((void***)&_self->Tables, &_self->TablesCount, newTbl, sizeof(TPTable*));
	_self->Tables = realloc(_self->Tables, sizeof(TPTable*) * (_self->TablesCount + 1));
	_self->Tables[_self->TablesCount] = newTbl;
	_self->TablesCount++;

	if(_self->Tables != NULL)
	{
		return TP_SUCCESS;
	}
	else
	{
		return TP_FAILED_AddTable;
	}
}

enum TP_ERROR_TYPES GetTable(TPDatabase *_self, char *_Name, int _col_count)
{
	char *_Name_TailSlash = TP_StrnCat("/", 1, _Name);
	TPTable *TableToAdd = CreateTPTable(_Name_TailSlash, _self, TP_FALSE);
	TableToAdd->ColCount = _col_count;
	free(_Name_TailSlash);

	TPTable_Conf *TableConf = ReadTableConfig(TableToAdd);
	TableToAdd->LastRowID = TableConf->LastId;
	TableToAdd->RowCount = TableConf->RowCount;
	TableToAdd->RowsOnDemand = TableConf->LazyLoad;
	free(TableConf);

	if(TableToAdd->RowsOnDemand == TP_FALSE)
	{
		TableToAdd->Rows = (TPTable_Row**)malloc(sizeof(TPTable_Row*) * (TableToAdd->RowCount));
		char **RowPaths = TP_GetFileNamesInDir(TableToAdd->Path, TableToAdd->RowCount);
		for (int i = 0; i < TableToAdd->RowCount; i++)
		{
			TableToAdd->Rows[i] = CreateTPTableRow(i, TableToAdd);
			char *tempVal = TP_ReadFile(RowPaths[i]);
			FreeArrayOfPointers((void***)&TableToAdd->Rows[i]->Values, TableToAdd->ColCount);
			TableToAdd->Rows[i]->Values = TP_SplitString(tempVal, ';', NULL);
			free(tempVal); tempVal = NULL;
		}
		FreeArrayOfPointers((void***)&RowPaths, TableToAdd->RowCount);
	}
	puts("her");

	_self->Tables = realloc(_self->Tables, sizeof(TPTable*) * (_self->TablesCount + 1));
	_self->Tables[_self->TablesCount] = TableToAdd;
	_self->TablesCount++;

	if(TableToAdd->RowsOnDemand == 0)
	{
		printf("v: %s\n", _self->Tables[_self->TablesCount - 1]->Rows[0]->Values[0]);
	}

	if(_self->Tables != NULL)
	{
		return TP_SUCCESS;
	}
	else
	{
		return TP_FAILED_AddTable;
	}
}
