#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Storage.h"
#include "../Errors/I_Errors.h"

enum TP_ERROR_TYPES TP_Mkdir(char *_path)
{
	if(ENABLE_ERROR_LOG){ printf("--|TP_TEST_Mkdir|--: ..."); }

	if(mkdir(_path, 0700) == -1)
	{
		if(ENABLE_ERROR_LOG){ printf("%s\n", ERROR_ASCII_WARNING); }
		return TP_FAILED_MKDIR;
	}

	if(ENABLE_ERROR_LOG){ printf("%s\n", ERROR_ASCII_SUCCESS); }
	return TP_SUCCESS;
}

enum TP_ERROR_TYPES TP_StoreFile(char *_path, char *_val)
{
	FILE *f = fopen(_path, "w");
	fputs(_val, f);
	fclose(f);
	return TP_SUCCESS;
}

char *TP_ReadFile(char* _path)
{
	FILE *f = fopen(_path, "r");
	fseek(f, 0, SEEK_END);

	size_t fLen = ftell(f);
	rewind(f);

	if(fLen <= 0)
	{
		fclose(f);
		return strdup("");
	}

	char *ret = (char*)malloc(sizeof(char) * (fLen + 1));
	
	if(fread(ret, sizeof(char), fLen, f) != fLen)
	{
		free(ret); ret = NULL;
		fclose(f);
		TP_CheckError(TP_FAILED_READFILE, TP_EXIT);
	}

	ret[fLen] = '\0';
	fclose(f);
	return ret;
}
