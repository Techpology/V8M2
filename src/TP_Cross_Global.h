#ifndef TP_CROSS_GLOBAL_H
#define TP_CROSS_GLOBAL_H

#include "../external/TPDBv2/src/Interface/I_Database.h"
#include "../external/TPDBv2/src/Interface/I_Table.h"
#include "../external/TPDBv2/src/Interface/I_Row.h"
#include "../external/TPDBv2/src/Interface/I_Indexing.h"

#define INCOMING_DIRECTORY "./Input/"

extern TPDatabase	*CrossDataBase;
extern TPTable		*CDFtbl;
extern TPTable		*CVFtbl;

typedef struct
{
	char *_name;
	char *_content;
	size_t _size;
} FILE_INCOMING;

static inline void Free_FILE_INCOMING(FILE_INCOMING **_f)
{
	free((*_f)->_name);
	free((*_f)->_content);
	free((*_f));
}

#endif
