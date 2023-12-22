#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "StringTools.h"
#include "../Errors/I_Errors.h"

char *TP_StrnCat(char *_baseString, int _count, ...)
{
	va_list args;
	va_start(args, _count);
	char *retStr = _baseString;

	for (size_t i = 0; i < _count; i++)
	{
		char *tempStr = va_arg(args, char*);
		int _strSize = snprintf(NULL, 0, "%s%s", retStr, tempStr) + 1;
		char *_str = (char *)malloc(sizeof(char) * _strSize);
		sprintf(_str, "%s%s", retStr, tempStr);

		if (retStr != _baseString && retStr != NULL)
		{
			free(retStr);
		}

		retStr = _str;
	}

	va_end(args);
	return retStr;
}

/*
	@brief Joins array of strings
	@exception {ERROR_CODE: 502}
	@attention Make sure to free the _list after this function.
	@attention TP_StrnCatArray does not free anything you pass.

	@param _list		String array
	@param _count		Count of strings in array
	@param _separator	What to seperate the strings with.

	@return {char*} The concatinated string of _list
*/
char *TP_StrnCatArray(char **_list, int _count, char *_separator)
{
	char *retStr = strdup(_list[0]);

	for (size_t i = 1; i < _count; i++)
	{
		char *tempStr = strdup(_list[i]);
		int _strSize = snprintf(NULL, 0, "%s%s%s", retStr, _separator, tempStr) + 1;
		char *_str = (char *)malloc(sizeof(char) * _strSize);
		sprintf(_str, "%s%s%s", retStr, _separator, tempStr);

		if (retStr != "" && retStr != NULL)
		{
			free(retStr);
		}

		retStr = _str;
		free(tempStr);
	}

	return retStr;
}

char **TP_SplitString(char *_str, char _separator, int *_returnCount)
{
	/*
		I personally haven't seen this implementation before.
		So I have to explain it so that opensource users can understand what it is I'm doing.

		Count how many tokens (_separator) there are.
		Allocate space for an integer array (int*) based on token instances found.
		Run through the string again and store the index of all instances of said token (_separator).
		Allocate space for a string array (char**) based on token instances found.

		#approach 1
		Increment to index[i] of token, set to '\0', and assign char* from -offset of index, to recent '\0'.
		This way you dont allocate any memory more than the original string you wanted to split,
		but still get to use it in a new data structure (char**). You only have to free (_str) and all members of
		(char**) will be freed thanks to that. Saves time and memory.

		#approach 2
		Strndup from index[i - 1] to index[i]. Assign as new (char*) in (char**).
	*/

	int tokenCount = 0;
	int _strLEN = strlen(_str);
	for (int i = 0; i < _strLEN; i++)
	{
		if(_str[i] == _separator){ tokenCount++; }
	}

	if(tokenCount <= 0){ TP_CheckError(TP_FAILED_SPLITSTRING, TP_IGNORE); return NULL; }

	int *tokensIndex = (int*)malloc(sizeof(int) * tokenCount);

	int j = 0;
	for (int i = 0; i < _strLEN; i++)
	{
		if(_str[i] == _separator)
		{
			tokensIndex[j] = i;
			j++;
		}
	}
	j = -1;
	char **ret = (char**)malloc(sizeof(char*) * (tokenCount + 1));

	for (int i = 0; i < tokenCount; i++)
	{
		if(i == 0)
		{
			ret[i] = strndup(_str, tokensIndex[i]);
		}
		else
		{
			ret[i] = strndup(_str + tokensIndex[i - 1] + 1, (tokensIndex[i]) - (tokensIndex[i - 1]) - 1);
		}
	}
	ret[tokenCount] = strndup(_str + tokensIndex[tokenCount - 1] + 1, 
		(tokenCount > 1) ? _strLEN - tokensIndex[tokenCount - 2] - 1 : _strLEN - tokensIndex[tokenCount - 1] - 1);
	if(_returnCount != NULL){ (*_returnCount) = tokenCount + 1; }

	free(tokensIndex);
	return ret;
}
