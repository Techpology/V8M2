#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./Database/DatabaseManager.h"
#include "./TP_Cross_Global.h"

#include "./Utils/MiscTools.h"

int main()
{
	CreateCrossDatabase();

	FILE_INCOMING *InputFilePath = FolderListener(INCOMING_DIRECTORY);

	Free_FILE_INCOMING(&InputFilePath);
	CloseCrossDatabase();
	exit(0);
}

