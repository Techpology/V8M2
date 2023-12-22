#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Database/DatabaseManager.h"
#include "./TP_Cross_Global.h"

#include "./Utils/MiscTools.h"

int main()
{
	CreateCrossDatabase();

	FILE_INCOMING *IncomingFILE = FolderListener(INCOMING_DIRECTORY);
	enum TP_INCOMING_FILE_TYPE _FORK_ = FORK_ToCompress_ToDecompress(IncomingFILE->_name);

	if(_FORK_ == TP_CROSS_FILE_COMPRESSED)
	{
		puts("INCOMING: {.ccf}, Iniating decompression");
	}
	else
	{
		puts("INCOMING: {.*}, Iniating compression");
	}

	Free_FILE_INCOMING(&IncomingFILE);
	CloseCrossDatabase();
	exit(0);
}

