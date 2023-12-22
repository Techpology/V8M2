#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../Errors/I_Errors.h"
#include "../Utils/StringTools.h"
#include "../Utils/SerializeTools.h"
#include "../Utils/MiscTools.h"

#include "../Storage/Storage.h"

#include "../Interface/I_Database.h"
#include "../Interface/I_Indexing.h"
#include "../Interface/I_Table.h"
#include "../Interface/I_Row.h"

enum TP_ERROR_TYPES TP_TEST_STRNCAT()
{
	printf("--|TP_TEST_STRNCAT|--: ...");
	char *test_1 = TP_StrnCat("", 2, "hello ", "world!");
	char *test_2 = TP_StrnCat("How ", 2, "are ", "you?");

	if(strcmp(test_1, "hello world!") == 0 && strcmp(test_2, "How are you?") == 0)
	{
		free(test_1);
		free(test_2);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		free(test_1);
		free(test_2);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_STRNCAT;
	}
}

enum TP_ERROR_TYPES TP_TEST_STRNCATARRAY()
{
	printf("--|TP_TEST_STRNCATARRAY|--: ...");
	char **tempArray = (char**)malloc(sizeof(char*) * 2);
	tempArray[0] = strdup("hello");
	tempArray[1] = strdup("world!");

	char *res = TP_StrnCatArray(tempArray, 2, " ");
	puts(res);

	if(res != NULL && strcmp(res, "hello world!") == 0)
	{
		free(tempArray[1]);
		free(tempArray[0]);
		free(tempArray);
		free(res);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		free(tempArray[1]);
		free(tempArray[0]);
		free(tempArray);
		free(res);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_STRNCATARRAY;
	}
}

enum TP_ERROR_TYPES TP_TEST_SPLITSTRING()
{
	printf("--|TP_TEST_SPLITSTRING|--: ...");
	char *tempStr = "hello,how,are,you, bro";

	int splitCount = 0;
	char **splitStr = TP_SplitString(tempStr, ',', &splitCount);

	if(strcmp(splitStr[0], "hello") == 0 && strcmp(splitStr[1], "how") == 0 && 
	strcmp(splitStr[2], "are") == 0 && strcmp(splitStr[3], "you") == 0
	)
	{
		for (int i = 0; i < splitCount; i++)
		{
			if(splitStr[i] != NULL){free(splitStr[i]); splitStr[i] = NULL;}
		}
		if(splitStr != NULL) { free(splitStr); }
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		for (int i = 0; i < splitCount; i++)
		{
			if(splitStr[i] != NULL){free(splitStr[i]); splitStr[i] = NULL;}
		}
		if(splitStr != NULL) { free(splitStr); }
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_SPLITSTRING;
	}
}

enum TP_ERROR_TYPES TP_TEST_AppendToArrayOfPointers()
{
	printf("--|TP_TEST_AppendToArrayOfPointers|--: ...");
	char **TestStrArray = NULL;
	size_t TestStrArrayLen = 0;

	char *str1 = "Hello";
	char *str2 = "World";
	TP_CheckError(AppendToArrayOfPointers((void***)&TestStrArray, &TestStrArrayLen, str1, sizeof(char*)), TP_EXIT);
	TP_CheckError(AppendToArrayOfPointers((void***)&TestStrArray, &TestStrArrayLen, str2, sizeof(char*)), TP_EXIT);

	if(strcmp(TestStrArray[0], "Hello") == 0 && strcmp(TestStrArray[1], "World") == 0 && TestStrArrayLen == 2)
	{
		if(TestStrArray[0] != NULL){ free(TestStrArray[0]); }
		if(TestStrArray[1] != NULL){ free(TestStrArray[1]); }
		if(TestStrArray != NULL){ free(TestStrArray); TestStrArray = NULL; }

		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		if(TestStrArray[0] != NULL){ free(TestStrArray[0]); }
		if(TestStrArray[1] != NULL){ free(TestStrArray[1]); }
		if(TestStrArray != NULL){ free(TestStrArray); TestStrArray = NULL; }

		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_AppendToArrayOfPointers;
	}
}

enum TP_ERROR_TYPES TP_TEST_FreeArrayOfPointers()
{
	printf("--|TP_TEST_FreeArrayOfPointers|--: ...");
	char **TestStrArray = NULL;
	size_t TestStrArrayLen = 0;

	char *str1 = "Hello";
	char *str2 = "World";
	TP_CheckError(AppendToArrayOfPointers((void***)&TestStrArray, &TestStrArrayLen, str1, sizeof(char*)), TP_EXIT);
	TP_CheckError(AppendToArrayOfPointers((void***)&TestStrArray, &TestStrArrayLen, str2, sizeof(char*)), TP_EXIT);

	TP_CheckError(FreeArrayOfPointers((void***)&TestStrArray, TestStrArrayLen), TP_EXIT);

	if(TestStrArray == NULL)
	{
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_FreeArrayOfPointers;
	}
}

enum TP_ERROR_TYPES TP_TEST_CreateTPDatabase()
{
	printf("--|TP_TEST_CreateTPDatabase|--: ...");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");

	if(MainDatabase != NULL)
	{
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_CREATETPDATABASE;
	}
}

enum TP_ERROR_TYPES TP_TEST_CreateTPTable()
{
	printf("--|TP_TEST_CreateTPTable|--: ...");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");
	TPTable *NewTable = CreateTPTable("/TestTable", MainDatabase, TP_FALSE);
	
	if(NewTable != NULL)
	{
		DestroyTPTable(NewTable);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		DestroyTPTable(NewTable);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_CREATETPTABLE;
	}
}

enum TP_ERROR_TYPES TP_TEST_CreateTPTableRow()
{
	printf("--|TP_TEST_CreateTPTableRow|--: ...");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");
	TPTable *NewTable = CreateTPTable("/TestTable", MainDatabase, TP_FALSE);
	NewTable->ColCount = 2;
	SetColumnTypes(NewTable, 2, TP_STRING, TP_STRING);
	TPTable_Row *NewRow = CreateTPTableRow(0, NewTable);

	if(NewRow != NULL)
	{
		DestroyTPTableRow(NewRow);
		DestroyTPTable(NewTable);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		DestroyTPTableRow(NewRow);
		DestroyTPTable(NewTable);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_CREATETPTABLEROW;
	}
}

enum TP_ERROR_TYPES TP_TEST_AddTable()
{
	printf("--|TP_TEST_AddTable|--: ...");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");
	AddTable(MainDatabase, "Users");

	if(MainDatabase != NULL)
	{
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_AddTable;
	}
}

enum TP_ERROR_TYPES TP_TEST_AddRow()
{
	printf("--|TP_TEST_AddRow|--: ...");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");
	
	AddTable(MainDatabase, "Users");
	SetColumnTypes(MainDatabase->Tables[0], 3, TP_STRING, TP_STRING, TP_INT);
	MainDatabase->Tables[0]->ColumnsIndexOffset = 10;
	AddIndexColumn(MainDatabase->Tables[0], 2);
	AddRow(MainDatabase->Tables[0], 3, "Ali", "123", 22);

	if(MainDatabase != NULL)
	{
		SyncTable(MainDatabase->Tables[0]);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_AddTable;
	}
}

enum TP_ERROR_TYPES TP_TEST_InsertRowToIndexTable()
{
	printf("--|TP_TEST_InsertRowToIndexTable|--: ...");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");
	
	AddTable(MainDatabase, "Users");
	SetColumnTypes(MainDatabase->Tables[0], 3, TP_STRING, TP_STRING, TP_INT);
	MainDatabase->Tables[0]->ColumnsIndexOffset = 10;
	AddIndexColumn(MainDatabase->Tables[0], 2);

	AddRow(MainDatabase->Tables[0], 3, "Ali", "123", 22);

	if(MainDatabase != NULL)
	{
		SyncTable(MainDatabase->Tables[0]);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_AddTable;
	}
}

enum TP_ERROR_TYPES TP_TEST_SERIALIZE_Str_Int()
{
	printf("--|TP_TEST_SERIALIZE_Str_Int|--: ...");
	char *value = "10";
	int *intValue = SERIALIZE_Str_Int(value);
	char *value1 = "-10";
	int *intValue1 = SERIALIZE_Str_Int(value1);
	if((*intValue) == 10 && (*intValue1) == -10)
	{
		free(intValue);
		free(intValue1);
		printf("%s\n",ERROR_ASCII_SUCCESS);
		return TP_SUCCESS;
	}
	else
	{
		free(intValue);
		free(intValue1);
		printf("%s\n",ERROR_ASCII_FAIL);
		return TP_FAILED_SERIALIZE_STR_INT;
	}

}

enum TP_ERROR_TYPES TP_TEST_SERIALIZE_Str_Float()
{
	printf("--|TP_TEST_SERIALIZE_Str_Float|--: ...");
	char *value = "10";
	float *floatValue = SERIALIZE_Str_Float(value);

	char *value1 = "-10";
	float *floatValue1 = SERIALIZE_Str_Float(value1);
	if((*floatValue) == 10 && (*floatValue1) == -10)
	{
		free(floatValue);
		free(floatValue1);
		printf("%s\n",ERROR_ASCII_SUCCESS);
		return TP_SUCCESS;
	}
	else
	{
		free(floatValue);
		free(floatValue1);
		printf("%s\n",ERROR_ASCII_FAIL);
		return TP_FAILED_SERIALIZE_STR_FLOAT;
	}

}

enum TP_ERROR_TYPES TP_TEST_GETROWVALUE()
{
	printf("--|TP_TEST_TP_TEST_GETROWVALUE|--: ...\n");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");

	AddTable(MainDatabase, "Users");
	SetColumnTypes(MainDatabase->Tables[0], 3, TP_STRING, TP_STRING, TP_INT);
	MainDatabase->Tables[0]->ColumnsIndexOffset = 10;
	AddIndexColumn(MainDatabase->Tables[0], 2);
	AddRow(MainDatabase->Tables[0], 3, "Ali", "123", 22);

	TPTable_Row *row = MainDatabase->Tables[0]->Rows[0];
	char *name = (char*)GetRowValue(MainDatabase->Tables[0], row, 0);
	int *age = (int*)GetRowValue(MainDatabase->Tables[0], row, 2);

	if(strcmp(name,"Ali") == 0 && (*age) == 22)
	{
		free(name);
		free(age);

		SyncTable(MainDatabase->Tables[0]);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		free(name);
		free(age);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_GETROWVALUE;
	}
}

enum TP_ERROR_TYPES TP_TEST_GetRow()
{
	printf("--|TP_TEST_GetRow|--: ...");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");

	AddTable(MainDatabase, "Users");
	MainDatabase->Tables[0]->RowsOnDemand = TP_TRUE;
	SetColumnTypes(MainDatabase->Tables[0], 3, TP_STRING, TP_STRING, TP_INT);
	MainDatabase->Tables[0]->ColumnsIndexOffset = 10;
	AddIndexColumn(MainDatabase->Tables[0], 2);

	// Do not free _row. It is just a pointer to a value in Table->Rows. It will get free'd with DestroyTPDatabase, etc...
	// If RowsOnDemand is true, then do not forget to free (DestroyTPTableRow()) the row. It has no link to the table and therefore needs to be freed.
	TPTable_Row *_row = GetRow(MainDatabase->Tables[0], 0);

	if(_row != NULL && strcmp(_row->Values[0], "Ali") == 0 && strcmp(_row->Values[1], "123") == 0 && strcmp(_row->Values[2], "22") == 0)
	{
		SyncTable(MainDatabase->Tables[0]);
		DestroyTPTableRow(_row);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		DestroyTPTableRow(_row);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_AddTable;
	}
}

enum TP_ERROR_TYPES TP_TEST_GetTable()
{
	printf("--|TP_TEST_GetTable|--: ...");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");

	GetTable(MainDatabase, "Users");
	SetColumnTypes(MainDatabase->Tables[0], 3, TP_STRING, TP_STRING, TP_INT);
	MainDatabase->Tables[0]->ColumnsIndexOffset = 10;
	AddIndexColumn(MainDatabase->Tables[0], 2);

	TPTable_Row *Row0 = GetRow(MainDatabase->Tables[0], 0);
	int *Row0Age = (int*)GetRowValue(MainDatabase->Tables[0], Row0, 2);
	printf("Row0Age: %d\n", (*Row0Age));
	free(Row0Age); Row0Age = NULL;
	DestroyTPTableRow(Row0);

	if(MainDatabase != NULL)
	{
		SyncTable(MainDatabase->Tables[0]);
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_GetTable;
	}
}

enum TP_ERROR_TYPES TP_TEST_GetIndexAtRange()
{
	printf("--|TP_TEST_GetIndexAtRange|--: ...");
	TPDatabase *MainDatabase = CreateTPDatabase("MainDatabase", "./db");

	GetTable(MainDatabase, "Users");
	MainDatabase->Tables[0]->RowsOnDemand = TP_TRUE;
	SetColumnTypes(MainDatabase->Tables[0], 3, TP_STRING, TP_STRING, TP_INT);
	MainDatabase->Tables[0]->ColumnsIndexOffset = 10;
	AddIndexColumn(MainDatabase->Tables[0], 2);

	int IndexIntArrLen = 0;
	int *IndexIntArr = TP_GetIndexAtRange(MainDatabase->Tables[0], 2, 21, &IndexIntArrLen);

	printf("Index result: ");
	for (int i = 0; i < IndexIntArrLen; i++)
	{
		printf("%d, ", IndexIntArr[i]);
	}
	printf("\n");

	if(IndexIntArr != NULL) { free(IndexIntArr); IndexIntArr = NULL; }
	if(MainDatabase != NULL)
	{
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_SUCCESS);
		printf("\n");
		return TP_SUCCESS;
	}
	else
	{
		DestroyTPDatabase(MainDatabase);
		printf(ERROR_ASCII_FAIL);
		printf("\n");
		return TP_FAILED_GetIndexAtRange;
	}
}

int main()
{
	puts("--|UnitTest|--");
	TP_CheckError(TP_TEST_STRNCAT(), TP_EXIT);
	TP_CheckError(TP_TEST_STRNCATARRAY(), TP_EXIT);
	TP_CheckError(TP_TEST_SPLITSTRING(), TP_EXIT);

	TP_CheckError(TP_TEST_AppendToArrayOfPointers(), TP_EXIT);
	TP_CheckError(TP_TEST_FreeArrayOfPointers(), TP_EXIT);

	TP_CheckError(TP_Mkdir("./db"), TP_IGNORE);
	TP_CheckError(TP_StoreFile("./db/test.txt", "TechP"), TP_EXIT);
	char *tempReadFile = TP_ReadFile("./db/test.txt");
	if(strcmp(tempReadFile, "TechP") != 0){ free(tempReadFile); TP_CheckError(TP_FAILED_READFILE, TP_EXIT); }
	free(tempReadFile);

	TP_CheckError(TP_TEST_CreateTPDatabase(), TP_EXIT);
	TP_CheckError(TP_TEST_CreateTPTable(), TP_EXIT);
	TP_CheckError(TP_TEST_CreateTPTableRow(), TP_EXIT);

	TP_CheckError(TP_TEST_AddTable(), TP_EXIT);
	TP_CheckError(TP_TEST_AddRow(), TP_EXIT);
	TP_CheckError(TP_TEST_InsertRowToIndexTable(), TP_EXIT);

	TP_CheckError(TP_TEST_SERIALIZE_Str_Int(), TP_EXIT);
	TP_CheckError(TP_TEST_SERIALIZE_Str_Float(), TP_EXIT);

	TP_CheckError(TP_TEST_GETROWVALUE(), TP_EXIT);

	TP_CheckError(TP_TEST_GetRow(), TP_EXIT);
	TP_CheckError(TP_TEST_GetTable(), TP_EXIT);

	TP_CheckError(TP_TEST_GetIndexAtRange(), TP_EXIT);

	exit(0);
}
