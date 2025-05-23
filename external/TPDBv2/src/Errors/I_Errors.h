#ifndef I_ERRORS_H
#define I_ERRORS_H

#define ERROR_ASCII_SUCCESS "✓"
#define ERROR_ASCII_WARNING "❓"
#define ERROR_ASCII_FAIL "☒"

#include "../TPDB_Global.h"
#define ENABLE_ERROR_LOG TP_TRUE

enum TP_ERROR_TYPES
{
	TP_SUCCESS = 0,
	
	TP_FAILED_CREATETPDATABASE = 101,
	TP_FAILED_AddTable = 102,
	TP_FAILED_AddIndexColumn = 103,
	TP_FAILED_GetTable = 104,
	
	TP_FAILED_CREATETPTABLE = 201,
	TP_FAILED_SetColumnTypes = 202,
	TP_FAILED_AddRow = 203,
	
	TP_FAILED_CREATETPTABLEROW = 301,
	TP_FAILED_UPDATEROW = 302,
	TP_FAILED_GETROWVALUE = 303,
	TP_FAILED_SetRowValues = 304,

	TP_FAILED_SERIALIZE_STR_INT = 401,
	TP_FAILED_SERIALIZE_STR_FLOAT = 402,

	TP_FAILED_STRNCAT = 501,
	TP_FAILED_STRNCATARRAY = 502,
	TP_FAILED_SPLITSTRING = 503,
	
	TP_FAILED_MKDIR = 601,
	TP_FAILED_READFILE = 602,
	
	TP_FAILED_AppendToArrayOfPointers = 701,
	TP_FAILED_FreeArrayOfPointers = 702,

	TP_FAILED_GetIndexAtRange = 801,
};

void TP_CheckError(enum TP_ERROR_TYPES err, int _exitOnError);

#endif
