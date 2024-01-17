#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Decompress.h"
#include "../TP_Cross_Global.h"
#include "../../external/TPDBv2/src/Utils/MiscTools.h"
#include "../../external/TPDBv2/src/Utils/SerializeTools.h"
#include "../../external/TPDBv2/src/Utils/StringTools.h"
#include "../../external/TPDBv2/src/Storage/Storage.h"
#include "../../external/TPDBv2/src/Interface/I_Database.h"
#include "../../external/TPDBv2/src/Interface/I_Table.h"

char *TP_CROSS_GetDecompressed(FILE_INCOMING *_ToCompress)
{
	int RefCount = 0;
	char **References_Injections = TP_SplitString(_ToCompress->_content, ',', &RefCount);

	for (int i = 0; i < RefCount; i++)
	{
		puts("LOOPING");
		puts(References_Injections[i]);
		int isRanged = 0;
		if(References_Injections[i][0] == 'i')
		{
			// ref
			puts("REF");
			int _i = 0;
			int _s = 0;
			int _l = 0;
			if(strstr(References_Injections[i], "s"))
			{
				isRanged = 1;
				char **Split_s = TP_SplitString(References_Injections[i], 's', NULL);
				printf("split_s: %s : %s\n", Split_s[0] + 1, Split_s[1]);
				char **Split_l = TP_SplitString(Split_s[1], 'l', NULL);
				printf("split_l: %s : %s\n", Split_l[0], Split_l[1]);

				_i = atoi(Split_s[0] + 1);
				_s = atoi(Split_l[0]);
				_l = atoi(Split_l[1]);
				FreeArrayOfPointers((void***)&Split_s, 2);
				FreeArrayOfPointers((void***)&Split_l, 2);
			}
			else
			{
				_i = atoi(References_Injections[i] + 1);
				_s = 0;
			}

			size_t srcPathSize = snprintf(NULL, 0, "./db/CDF/%d.tdf", _i) + 1;
			char *srcPath = (char*)malloc(sizeof(char) * srcPathSize);
			sprintf(srcPath, "./db/CDF/%d.tdf", _i);

			char *SrcHex = TP_ReadFile(srcPath);
			_l = (isRanged == 0) ? strlen(SrcHex) / 2 : _l;

			free(References_Injections[i]);
			References_Injections[i] = (char*)malloc(sizeof(char) * _l * 2);
			strncpy(References_Injections[i], SrcHex + (_s * 2), _l * 2);

			free(srcPath); srcPath = NULL;
			free(SrcHex); SrcHex = NULL;
		}
	}
	char *toRet = TP_StrnCatArray(References_Injections, RefCount, "");

	FreeArrayOfPointers((void***)&References_Injections, RefCount);
	return toRet;
}

/* char *TP_CROSS_GetDecompressed(FILE_INCOMING *_ToCompress)
{
	int RefCount = 0;
	char **References_Injections = TP_SplitString(_ToCompress->_content, ',', &RefCount);

	char *ToRet = (char*)malloc(sizeof(char) * atoi(References_Injections[RefCount - 1]));
	int activeIndex = 0;

	for (int i = 0; i < RefCount; i++)
	{
		if(References_Injections[i][0] != 'i')
		{
			int _a = strlen(References_Injections[i]);
			strncpy(ToRet + activeIndex, References_Injections[i], _a);
			activeIndex += _a;
		}
		else
		{
			char **splitFirst = TP_SplitString(References_Injections[i], 's', NULL);
			char **splitSecond = TP_SplitString(splitFirst[1], 'l', NULL);

			char *_id = (splitFirst[0]) + 1;
			char *_start = splitSecond[0];
			char *_len = splitSecond[1];

			int i_id = atoi(_id);
			int i_s = atoi(_start);
			int i_l = atoi(_len);

			char *Source_path	= TP_StrnCat(CDFtbl->Path, 3, "/", _id, ".tdf");
			char *Source_hex	= TP_ReadFile(Source_path);

			char *nStr = (char*)malloc((sizeof(char) * (i_l * 2)) + 1);
	  		strncpy(nStr, Source_hex + (i_s * 2), (i_l * 2));

			strncpy(ToRet + (activeIndex), nStr, strlen(nStr));
			activeIndex += i_l * 2;

			free(nStr);
			free(Source_path);
			free(Source_hex);
			FreeArrayOfPointers((void***)&splitFirst, 2);
			FreeArrayOfPointers((void***)&splitSecond, 2);
		}
	}
	
	FreeArrayOfPointers((void***)&References_Injections, RefCount);
	return ToRet;
} */
