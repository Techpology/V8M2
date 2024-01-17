#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Database/DatabaseManager.h"
#include "./TP_Cross_Global.h"

#include "./Utils/MiscTools.h"
#include "./Process/Compress.h"
#include "./Process/Decompress.h"

#include "../external/TPDBv2/src/Storage/Storage.h"
#include "../external/TPDBv2/src/Utils/StringTools.h"
#include "../external/TPDBv2/src/Utils/MiscTools.h"

int main()
{
	//CreateCrossDatabase();
	GetCrossDatabase();
	ParseCVF();
	//START:

	FILE_INCOMING *IncomingFILE = FolderListener(INCOMING_DIRECTORY);
	enum TP_INCOMING_FILE_TYPE _FORK_ = FORK_ToCompress_ToDecompress(IncomingFILE->_name);

	if(_FORK_ == TP_CROSS_FILE_COMPRESSED)
	{
		puts("INCOMING: {.ccf}, Iniating decompression");
		char *decompressedStr = TP_CROSS_GetDecompressed(IncomingFILE);
		
		int SplitSize = 0;
		char **SplitName = TP_SplitString(IncomingFILE->_name, '.', &SplitSize);
		char *DecompName = TP_StrnCat(SplitName[0], 2, ".", SplitName[1]);
		FreeArrayOfPointers((void***)&SplitName, SplitSize);

		size_t TargetPathSize = snprintf(NULL, 0, "%s%s", OUTPUT_DECOMPRESS_DIRECTORY, DecompName) + 1;
		char *TargetPath = (char*)malloc(sizeof(char) * TargetPathSize);
		sprintf(TargetPath, "%s%s", OUTPUT_DECOMPRESS_DIRECTORY, DecompName);
		TP_StoreFile(TargetPath, decompressedStr);

		free(TargetPath); TargetPath = NULL;
		free(DecompName); DecompName = NULL;
		free(decompressedStr); decompressedStr = NULL;
	}
	else
	{
		puts("INCOMING: {.*}, Iniating compression");
		char *compressedStr = TP_CROSS_GetCompressed(IncomingFILE);

		size_t TargetPathSize = snprintf(NULL, 0, "%s%s.ccf", OUTPUT_COMPRESS_DIRECTORY, IncomingFILE->_name) + 1;
		char *TargetPath = (char*)malloc(sizeof(char) * TargetPathSize);
		sprintf(TargetPath, "%s%s.ccf", OUTPUT_COMPRESS_DIRECTORY, IncomingFILE->_name);
		TP_StoreFile(TargetPath, compressedStr);

		free(compressedStr); compressedStr = NULL;
		free(TargetPath); TargetPath = NULL;
	}

	//goto START;
	Free_FILE_INCOMING(&IncomingFILE);
	CloseCrossDatabase();
	exit(0);
}
