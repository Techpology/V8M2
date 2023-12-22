#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/TPDBv2/src/Utils/SerializeTools.h"
#include "../../external/TPDBv2/src/Utils/StringTools.h"
#include "../../external/TPDBv2/src/Utils/MiscTools.h"

#include "./CrossSerializers.h"

char *CrossSerializer_IntArr_Str(int *_arr, int _count)
{
	char **ArrStr = (char**)malloc(sizeof(char*) * _count);
	for (int i = 0; i < _count; i++)
	{
		char *ValStr = SERIALIZE_Int_Str(_arr[i]);
		ArrStr[i] = ValStr;
	}

	char *ToRet = TP_StrnCatArray(ArrStr, _count, ",");

	FreeArrayOfPointers((void***)&ArrStr, _count);
	return ToRet;
}

