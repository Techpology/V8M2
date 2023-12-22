#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../Errors/I_Errors.h"
#include "../Utils/StringTools.h"
#include "../Storage/Storage.h"
#include "../Utils/SerializeTools.h"

#include "I_Row.h"

TPTable_Row *CreateTPTableRow(int __ID, TPTable *_Table)
{
	TPTable_Row *newTPTR = (TPTable_Row*)malloc(sizeof(TPTable_Row));
	newTPTR->_ID = __ID;
	newTPTR->ValCount = _Table->ColCount;
	newTPTR->Values = (char**)malloc(sizeof(char*) * (newTPTR->ValCount));

	for (int i = 0; i < newTPTR->ValCount; i++)
	{
		newTPTR->Values[i] = NULL;
	}

	return newTPTR;
}

void DestroyTPTableRow(TPTable_Row *_self)
{
	if(_self != NULL)
	{
		_self->_ID = -1;

		if(_self->Values != NULL)
		{
			for (int i = 0; i < _self->ValCount; i++)
			{
				if(_self->Values[i] != NULL)
				{
					free(_self->Values[i]);
					_self->Values[i] = NULL;
				}
			}
			free(_self->Values); _self->Values = NULL;
		}
		free(_self); _self = NULL;
	}
}

void EmptyRowValues(TPTable_Row *_self)
{
	if(_self != NULL)
	{
		if(_self->Values != NULL)
		{
			for (int i = 0; i < _self->ValCount; i++)
			{
				if(_self->Values[i] != NULL)
				{
					free(_self->Values[i]);
					_self->Values[i] = NULL;
				}
			}
		}
	}
}

enum TP_ERROR_TYPES UpdateRow(TPTable *_parent, TPTable_Row *_self)
{
	char *ToStore 	= TP_StrnCatArray(_self->Values, _self->ValCount, ";");
	if(ToStore == NULL){ return TP_FAILED_UPDATEROW; }

	size_t idSize	= snprintf(NULL, 0, "/%d", _self->_ID) + 1;
	char *idStr		= (char*) malloc(sizeof(char) * idSize);
	sprintf(idStr, "/%d", _self->_ID);
	if(idStr == NULL){ return TP_FAILED_UPDATEROW; }

	char *Path		= TP_StrnCat(_parent->Path, 2, idStr, TP_ROW_EXT);
	if(Path == NULL){ return TP_FAILED_UPDATEROW; }
	TP_StoreFile(Path, ToStore);

	free(idStr);
	free(Path);
	free(ToStore);
	if(_parent->RowsOnDemand == TP_TRUE){ EmptyRowValues(_self); }
	return TP_SUCCESS;
}

/**
 * TODO
 * Implement for all types
 * */
void *GetRowValue(TPTable *table,TPTable_Row *row, int column){

	char *value = row->Values[column];

	switch(table->ColumnTypes[column]){
		case TP_STRING:
			char* strVal = (char*)malloc(sizeof(char)*(strlen(value)+1));
			strcpy(strVal,value);
			strVal[strlen(value)] = '\0';
			return strVal;

		case TP_INT:
			return SERIALIZE_Str_Int(value);

		case TP_FLOAT:
			return SERIALIZE_Str_Float(value);

		case TP_FKEY:
			return SERIALIZE_Str_Row(table->ParentDatabase,value);
	}

	return NULL;
}

enum TP_ERROR_TYPES SetRowValues(TPTable *_parent, TPTable_Row *_self, int _count, ...)
{
	va_list args;
	va_start(args, _count);

	for (int i = 0; i < _parent->ColCount; i++)
	{
		switch (_parent->ColumnTypes[i])
		{
			case TP_INT:
				int IntVal = va_arg(args, int);
				if(IntVal != TP_KEEP_ROW_VAL_NUM)
				{
					free(_self->Values[i]);
					_self->Values[i] = SERIALIZE_Int_Str(IntVal);
				}
				break;
			case TP_FLOAT:
				double FloatVal = va_arg(args, double);
				if(FloatVal != TP_KEEP_ROW_VAL_NUM)
				{
					free(_self->Values[i]);
					_self->Values[i] = SERIALIZE_Float_Str(FloatVal);
				}
				break;
			case TP_CHAR:
				unsigned char CharVal = (unsigned char)va_arg(args, int);
				if(CharVal != TP_KEEP_ROW_VAL_NUM)
				{
					free(_self->Values[i]);
					_self->Values[i] = SERIALIZE_Char_Str(CharVal);
				}
				break;
			case TP_FKEY:
				TPForeignKey *FKeyVal = va_arg(args, TPForeignKey*);
				if(FKeyVal != TP_KEEP_ROW_VAL_PTR)
				{
					free(_self->Values[i]);
					_self->Values[i] = SERIALIZE_Fkey_Str(FKeyVal);
				}
				break;
			default:
				char *StrVal = va_arg(args, char*);
				if(StrVal != TP_KEEP_ROW_VAL_PTR)
				{
					free(_self->Values[i]);
					StrVal = strdup(StrVal);
					_self->Values[i] = StrVal;
				}
				break;
		}
		if(_self->Values[i] == NULL)
		{
			va_end(args);
			return TP_FAILED_SetRowValues;
		}
	}
	va_end(args);
	return TP_SUCCESS;
}
