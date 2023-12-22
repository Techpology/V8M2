#include <stdio.h>
#include <stdlib.h>

#include "I_Cross_Errors.h"

void TPCross_CheckError(enum TPCROSS_ERROR_TYPES err, int _exitOnError)
{
	if(err != TPCROSS_SUCCESS)
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