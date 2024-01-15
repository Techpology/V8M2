#ifndef TP_CROSS_GLOBAL_H
#define TP_CROSS_GLOBAL_H

#include "../external/TPDBv2/src/Interface/I_Database.h"
#include "../external/TPDBv2/src/Interface/I_Table.h"
#include "../external/TPDBv2/src/Interface/I_Row.h"
#include "../external/TPDBv2/src/Interface/I_Indexing.h"

#define INCOMING_DIRECTORY "./COMP/"
#define OUTPUT_COMPRESS_DIRECTORY "./CF/"
#define OUTPUT_DECOMPRESS_DIRECTORY "./DF/"
//Minimum search similarity (COSsim)
#define SEARCH_MIN_SIM 0.7

extern TPDatabase	*CrossDataBase;
extern TPTable		*CDFtbl;
extern TPTable		*CVFtbl;
extern int			**CVF_Vals_Global;

enum TP_INCOMING_FILE_TYPE
{
	TP_CROSS_FILE_COMPRESSED,
	TP_CROSS_FILE_ANY
};

enum TP_CROSS_Query_ResultState
{
	TP_CQRS_FOUND,
	TP_CQRS_NONE
};

typedef struct
{
	enum TP_CROSS_Query_ResultState queryState;
	size_t querySize;
	int *queryResults;
} TP_CROSS_QueryObject;

static inline void Free_TP_CROSS_QueryObject(TP_CROSS_QueryObject **_f)
{
	free((*_f)->queryResults);
	free((*_f));
}

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

typedef struct
{
	int isReference;
	size_t startIndex;
	size_t endIndex;
} TP_CROSS_ReferenceObj;

#endif
