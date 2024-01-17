#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "I_Errors.h"

void TP_CheckError(enum TP_ERROR_TYPES err, int _exitOnError)
{
	if(err != TP_SUCCESS && err != TP_FAILED_MKDIR && err != TP_FAILED_SPLITSTRING)
	{
		int ErrorDocLinkSize = snprintf(NULL, 0, "firefox https://techpology.github.io/TPDB_V2/Errors/TPErrors.html?%d", err) + 1;
		char *ErrorDocLink = (char*)malloc(sizeof(char) * ErrorDocLinkSize);
		sprintf(ErrorDocLink, "firefox https://techpology.github.io/TPDB_V2/Errors/TPErrors.html?%d", err);

		puts("You can enable auto redirect within the source code in:\n{/src/Errors/I_Errors.c : (TP_CheckError())}, uncomment system(...)\n");
		puts(ErrorDocLink);
		puts("");
		//system(ErrorDocLink);
		free(ErrorDocLink);
		if(_exitOnError == TP_EXIT){ exit(err); }
	}
}
