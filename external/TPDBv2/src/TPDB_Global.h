#ifndef TPDB_GLOBAL_H
#define TPDB_GLOBAL_H
#include <stdio.h>
#include <math.h>

#define TP_TRUE 1
#define TP_FALSE 0

#define TP_EXIT 1
#define TP_IGNORE 0

#define TP_ROW_EXT ".tdf"
#define TP_KEEP_ROW_VAL_NUM NAN
#define TP_KEEP_ROW_VAL_PTR NULL

enum TPTable_Column_Types
{
	TP_INT,
	TP_FLOAT,
	TP_STRING,
	TP_CHAR,
	TP_FKEY
};

typedef struct
{
	char *TableName;
	int _ID;
} TPForeignKey;

typedef struct TPDatabase TPDatabase;

typedef struct
{
	int _ID;
	int ValCount;
	char **Values;
} TPTable_Row;

typedef struct 
{
	int LastId;
	int RowCount;
	int LazyLoad;
} TPTable_Conf;


typedef struct
{
	TPDatabase *ParentDatabase;
	int _ID;
	char *Name;
	char *Path;

	int ColCount;
	int RowCount;
	int LastRowID;

	int ColumnsIndexOffset;
	int ColumnsToIndexCount;
	int *ColumnsToIndex;

	enum TPTable_Column_Types *ColumnTypes;
	TPTable_Row **Rows;

	int RowsOnDemand;
} TPTable;

struct TPDatabase
{
	int _ID;
	char *Name;
	char *Path;
	char *ConfigPath;
	size_t TablesCount;
	TPTable **Tables;
};

#endif