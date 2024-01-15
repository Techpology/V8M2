#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Decompress.h"
#include "../../external/TPDBv2/src/Utils/MiscTools.h"
#include "../../external/TPDBv2/src/Utils/SerializeTools.h"
#include "../../external/TPDBv2/src/Utils/StringTools.h"

char *TP_CROSS_GetDecompressed(FILE_INCOMING *_ToCompress)
{
	int RefCount = 0;
	char **References_Injections = TP_SplitString(_ToCompress->_content, ',', &RefCount);

	char *ToRet = (char*)malloc(sizeof(char) * atoi(References_Injections[RefCount - 1]));
	int activeIndex = 0;

	for (int i = 0; i < RefCount; i++)
	{
		if(References_Injections[i][0] != 'i')
		{
			
		}
		else
		{
			// Reference
		}
	}
	
}
