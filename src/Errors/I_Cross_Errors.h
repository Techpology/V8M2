#ifndef I_CROSS_ERRORS_H
#define I_CROSS_ERRORS_H

#include <stdlib.h>

#include "../../external/TPDBv2/src/TPDB_Global.h"
#include "../../external/TPDBv2/src/Errors/I_Errors.h"

#define ExceptIfNULL(excep, a) (((a) == (NULL)) ? (excep) : (1000) )
#define IfZero(a, b) (((a) <= (0)) ? (b) : (a))

enum TPCROSS_ERROR_TYPES
{
	TPCROSS_SUCCESS = 1000,
	
	TP_FAILED_CreateCrossDatabase = 1101,
	TP_FAILED_GETCrossDatabase = 1102,
	TP_FAILED_AddCDF = 1103,

	//CUDA
	TP_FAILED_Array1D_Cuda_KernelMap = 2101,
	TP_FAILED_TPCUDA_Alloc_Str = 2102,
	TP_FAILED_TPCUDA_GetStr = 2103,
	TP_FAILED_TPCUDA_Free_Str = 2104,

	TP_FAILED_BytesToHex = 2201,
};

void TPCross_CheckError(enum TPCROSS_ERROR_TYPES err, int _exitOnError);

static inline void IfnNullFree(void **a) { if((*a) != NULL){ free((*a)); (*a) = NULL; } }

#endif
