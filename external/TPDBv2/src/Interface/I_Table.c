#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../Errors/I_Errors.h"
#include "../Utils/StringTools.h"
#include "../Utils/SerializeTools.h"
#include "../Utils/MiscTools.h"
#include "../Storage/Storage.h"
#include "I_Table.h"
#include "I_Row.h"
#include "I_Indexing.h"

TPTable *CreateTPTable(char *_Name, TPDatabase *_Database, int _lazyLoadRows)
{
	TPTable *newTPT = (TPTable*)malloc(sizeof(TPTable));
	newTPT->ParentDatabase = _Database;
	
	newTPT->_ID  = -1;
	newTPT->Name = strdup(_Name);
	newTPT->Path = TP_StrnCat(_Database->Path, 1, newTPT->Name);

	newTPT->ColumnsToIndex = NULL;
	newTPT->ColumnsToIndexCount = 0;
	newTPT->ColumnsIndexOffset = 1000;

	newTPT->ColumnTypes = NULL;
	
	newTPT->ColCount = 0;
	newTPT->RowCount = 0;
	newTPT->LastRowID = 0;

	newTPT->Rows = NULL;
	newTPT->RowsOnDemand = _lazyLoadRows;

	TP_CheckError(TP_Mkdir(newTPT->Path), TP_EXIT);

	return newTPT;
}

void DestroyTPTable(TPTable *_self)
{
	if(_self != NULL)
	{
		free(_self->ColumnsToIndex);
		_self->ColumnsToIndexCount = 0;
		if(_self->Rows != NULL)
		{
			for (int i = 0; i < _self->RowCount; i++)
			{
				DestroyTPTableRow(_self->Rows[i]);
			}
			free(_self->Rows);
		}
		if(_self->ColumnTypes != NULL)
		{
			free(_self->ColumnTypes);
			_self->ColCount = 0;
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
		_self->ParentDatabase = NULL;
		free(_self);
	}
}

enum TP_ERROR_TYPES SetColumnTypes(TPTable *_self, int _count, ...)
{
	_self->ColumnTypes = (enum TPTable_Column_Types*)malloc(sizeof(enum TPTable_Column_Types) * _count);
	if(_self->ColumnTypes == NULL)
	{
		return TP_FAILED_SetColumnTypes;
	}

	va_list args;
	va_start(args, _count);

	for (int i = 0; i < _count; i++)
	{
		_self->ColumnTypes[i] = va_arg(args, enum TPTable_Column_Types);
	}
	va_end(args);
	_self->ColCount = _count;
	return TP_SUCCESS;
}

enum TP_ERROR_TYPES AddRow(TPTable *_self, int _count, ...)
{
	if(_self->ColumnTypes == NULL || _self->ColCount <= 0){ return TP_FAILED_AddRow; }
	_count = _self->ColCount;

	TPTable_Row *NewRow = CreateTPTableRow(_self->LastRowID, _self);

	va_list args;
	va_start(args, _count);

	for (int i = 0; i < _self->ColCount; i++)
	{
		switch (_self->ColumnTypes[i])
		{
			case TP_INT:
				int _int = va_arg(args, int);
				NewRow->Values[i] = SERIALIZE_Int_Str(_int);
				break;
			case TP_FLOAT:
				double _float = va_arg(args, double);
				NewRow->Values[i] = SERIALIZE_Float_Str(_float);
				break;
			case TP_CHAR:
				unsigned char _char = (unsigned char)va_arg(args, int);
				NewRow->Values[i] = SERIALIZE_Char_Str(_char);
				break;
			case TP_FKEY:
				TPForeignKey* _TPForeignKey = va_arg(args, TPForeignKey*);
				if(_TPForeignKey != NULL)
				{
					NewRow->Values[i] = SERIALIZE_Fkey_Str(_TPForeignKey);
				}else
				{
					NewRow->Values[i] = strdup("NULL");
				}
				break;
			default:
				char *_charp = va_arg(args, char*);
				NewRow->Values[i] = strdup(_charp);
				break;
		}
	}
	va_end(args);

	if(NewRow != NULL)
	{
		if(_self->RowsOnDemand == TP_FALSE)
		{
			_self->Rows = realloc(_self->Rows, sizeof(TPTable_Row) * (_self->RowCount + 1));
			_self->Rows[_self->RowCount] = NewRow;
		}
		_self->RowCount++;
		_self->LastRowID++;
	}
	else
	{
		return TP_FAILED_AddRow;
	}

	// ########### Index ########### //
	
	if(_self->ColumnsToIndexCount > 0)
	{
		for (size_t i = 0; i < _self->ColumnsToIndexCount; i++)
		{
			int colIndex = _self->ColumnsToIndex[i];
			if(_self->ColumnTypes[colIndex] == TP_INT || _self->ColumnTypes[colIndex] == TP_FLOAT)
			{
				char *targetRangeStr = TP_GetIntRangeStr(_self->ColumnsIndexOffset, atoi(NewRow->Values[colIndex]));
				TP_CheckError(TP_InsertRowToIndexTable(_self, NewRow, targetRangeStr, colIndex), TP_EXIT);
				free(targetRangeStr);
			}
			// No string indexing implementation. Later update.
		}
	}

	if(_self->RowsOnDemand == TP_FALSE)
	{
		UpdateRow(_self, _self->Rows[_self->RowCount - 1]);
	}
	else
	{
		UpdateRow(_self, NewRow);
	}

	if(_self->RowsOnDemand == TP_TRUE)
	{
		DestroyTPTableRow(NewRow);
	}

	return TP_SUCCESS;
}

enum TP_ERROR_TYPES AddIndexColumn(TPTable *_self, int _col)
{
	_self->ColumnsToIndex = realloc(_self->ColumnsToIndex, sizeof(int) * (_self->ColumnsToIndexCount + 1));
	if(_self->ColumnsToIndex == NULL){ return TP_FAILED_AddIndexColumn; }

	_self->ColumnsToIndex[_self->ColumnsToIndexCount] = _col;
	_self->ColumnsToIndexCount++;

	return TP_SUCCESS;
}

TPTable_Row *GetRow(TPTable *_self, int _row)
{
	if(_self->RowsOnDemand == TP_FALSE)
	{
		return _self->Rows[_row];
	}

	char *RowPath = SERIALIZE_RowID_Path(_self, _row);
	char *RowStr  = TP_ReadFile(RowPath);
	char **RowSplit = TP_SplitString(RowStr, ';', NULL);

	TPTable_Row *NewRow = CreateTPTableRow(_row, _self);

	for (int i = 0; i < _self->ColCount; i++)
	{
		NewRow->Values[i] = strdup(RowSplit[i]);
	}
	NewRow->ValCount = _self->ColCount;

	if(RowPath != NULL){ free(RowPath); RowPath = NULL; }
	if(RowStr != NULL){ free(RowStr); RowStr = NULL; }
	if(RowSplit != NULL){ FreeArrayOfPointers((void***)&RowSplit, _self->ColCount); }
	return NewRow;
}

enum TP_ERROR_TYPES SyncTable(TPTable *_self)
{
	size_t ConfPathSize = snprintf(NULL, 0, "%s%s/Conf.txt", _self->ParentDatabase->ConfigPath, _self->Name) + 1;
	char *ConfPath = (char*)malloc(sizeof(char) * ConfPathSize);
	sprintf(ConfPath, "%s%s/Conf.txt", _self->ParentDatabase->ConfigPath, _self->Name);

	size_t ConfValSize = snprintf(NULL, 0, "LastId:%d\nRowCount:%d\nLazyLoad:%d", _self->LastRowID, _self->RowCount, _self->RowsOnDemand) + 1;
	char *ConfVal = (char*)malloc(sizeof(char) * ConfValSize);
	sprintf(ConfVal, "LastId:%d\nRowCount:%d\nLazyLoad:%d", _self->LastRowID, _self->RowCount, _self->RowsOnDemand);

	TP_StoreFile(ConfPath, ConfVal);

	free(ConfPath);
	free(ConfVal);
}
