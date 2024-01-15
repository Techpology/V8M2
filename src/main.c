#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Database/DatabaseManager.h"
#include "./TP_Cross_Global.h"

#include "./Utils/MiscTools.h"
#include "./Process/Compress.h"

#include "../external/TPDBv2/src/Storage/Storage.h"

int main()
{
	//CreateCrossDatabase();
	GetCrossDatabase();
	ParseCVF();

	FILE_INCOMING *IncomingFILE = FolderListener(INCOMING_DIRECTORY);
	enum TP_INCOMING_FILE_TYPE _FORK_ = FORK_ToCompress_ToDecompress(IncomingFILE->_name);

	if(_FORK_ == TP_CROSS_FILE_COMPRESSED)
	{
		puts("INCOMING: {.ccf}, Iniating decompression");
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

	Free_FILE_INCOMING(&IncomingFILE);
	CloseCrossDatabase();
	exit(0);
}
