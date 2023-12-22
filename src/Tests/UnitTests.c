#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../GpuAccelerated/Cross_Cuda_Utils.hu"
#include "../GpuAccelerated/I_Cross_Cuda.hu"

#include "../Errors/I_Cross_Errors.h"
#include "../Database/DatabaseManager.h"

// 0:f || 1:t
#define EnableCudaUnitTesting 1

// #--------- Cuda ---------# //
enum TPCROSS_ERROR_TYPES TPCROSS_TEST_Array1D_Cuda_KernelMap()
{
	printf("--|TPCROSS_TEST_Array1D_Cuda_KernelMap|--: ...");
	int ThreadsNeeded	= 0;
	int BlocksNeeded	= 0;
	Array1D_Cuda_KernelMap(&ThreadsNeeded, &BlocksNeeded, 10000);
	if(ThreadsNeeded != 1 || BlocksNeeded != 1){ printf("%s\n", ERROR_ASCII_FAIL); return TP_FAILED_Array1D_Cuda_KernelMap; }

	int ThreadsNeeded2	= 0;
	int BlocksNeeded2	= 0;
	Array1D_Cuda_KernelMap(&ThreadsNeeded2, &BlocksNeeded2, 100000);
	if(ThreadsNeeded2 != 10 || BlocksNeeded2 != 1){ printf("%s\n", ERROR_ASCII_FAIL); return TP_FAILED_Array1D_Cuda_KernelMap; }

	int ThreadsNeeded3	= 0;
	int BlocksNeeded3	= 0;
	Array1D_Cuda_KernelMap(&ThreadsNeeded3, &BlocksNeeded3, 5100000);
	if(ThreadsNeeded3 != 500 || BlocksNeeded3 != 2){ printf("%s\n", ERROR_ASCII_FAIL); return TP_FAILED_Array1D_Cuda_KernelMap; }

	printf("%s\n", ERROR_ASCII_SUCCESS);
	return TPCROSS_SUCCESS;
}

enum TPCROSS_ERROR_TYPES TPCROSS_TEST_TPCUDA_Alloc_Get_Free_Str()
{
	printf("--|TPCROSS_TEST_TPCUDA_Alloc_Get_Free_Str|--: ...");
	char *ToAlloc	= strdup("Hello world");
	char *Alloced_D	= TPCUDA_Alloc_Str(ToAlloc, strlen(ToAlloc));
	if(ExceptIfNULL(TP_FAILED_TPCUDA_Alloc_Str, ToAlloc) == TP_FAILED_TPCUDA_Alloc_Str)
	{TPCUDA_Free_Str(Alloced_D); IfnNullFree((void**)&ToAlloc); printf("%s\n", ERROR_ASCII_FAIL); return TP_FAILED_TPCUDA_Alloc_Str;}
	
	free(ToAlloc);
	ToAlloc = TPCUDA_GetStr(Alloced_D, strlen("Hello world"));

	if(strcmp(ToAlloc, "Hello world") != 0)
	{ TPCUDA_Free_Str(Alloced_D); IfnNullFree((void**)&ToAlloc); printf("%s\n", ERROR_ASCII_FAIL); return TP_FAILED_TPCUDA_GetStr; }
	
	TPCUDA_Free_Str(Alloced_D);
	IfnNullFree((void**)&ToAlloc);
	printf("%s\n", ERROR_ASCII_SUCCESS);
	return TPCROSS_SUCCESS;
}

enum TPCROSS_ERROR_TYPES TPCROSS_TEST_BytesToHex()
{
	printf("--|TPCROSS_TEST_BytesToHex|--: ...");

	char* ToAlloc = strdup("hello");
	size_t ToAllocSize = strlen(ToAlloc);

	char *_DeviceResult = TPCross_BytesToHex(ToAlloc, ToAllocSize);
	if(strcmp(_DeviceResult, "68656c6c6f") != 0)
	{IfnNullFree((void**)&ToAlloc); IfnNullFree((void**)&_DeviceResult); printf("%s\n", ERROR_ASCII_FAIL); return TP_FAILED_BytesToHex;}

	IfnNullFree((void**)&ToAlloc);
	IfnNullFree((void**)&_DeviceResult);
	printf("%s\n", ERROR_ASCII_SUCCESS);
	return TPCROSS_SUCCESS;
}

enum TPCROSS_ERROR_TYPES TPCROSS_TEST_HexVectorize()
{
	printf("--|TPCROSS_TEST_HexVectorize|--: ...");

	char *ToHexify = strdup("Hello world");
	size_t ToHexifySize = strlen(ToHexify);

	char *_Device_HexifyRes = TPCross_BytesToHex(ToHexify, ToHexifySize);
	int *_Device_Res = TPCross_HexVectorize(_Device_HexifyRes, strlen(_Device_HexifyRes));

	// printf("Vectors: ");
	// for (int i = 0; i < 256; i++)
	// {
	// 	printf("%d, ", _Device_Res[i]);
	// }
	// printf("\n");

	IfnNullFree((void**)&ToHexify);
	IfnNullFree((void**)&_Device_HexifyRes);
	IfnNullFree((void**)&_Device_Res);
	printf("%s\n", ERROR_ASCII_SUCCESS);
	return TPCROSS_SUCCESS;
}
// #--------- Main ---------# //

enum TPCROSS_ERROR_TYPES TPCROSS_TEST_AddCDF()
{
	printf("--|TPCROSS_TEST_AddCDF|--: ...");
	char *CDFVal = strdup("000000000000000000");
	TPCross_CheckError(AddCDF(CDFVal), TP_EXIT);

	free(CDFVal); CDFVal = NULL;
	printf("%s\n", ERROR_ASCII_SUCCESS);
	return TPCROSS_SUCCESS;
}

int main()
{
	// #--------- Cuda ---------# //
	// CreateCrossDatabase();
	GetCrossDatabase();
	if(EnableCudaUnitTesting == 1)
	{
		TPCross_CheckError(TPCROSS_TEST_Array1D_Cuda_KernelMap(), TP_EXIT);
		TPCross_CheckError(TPCROSS_TEST_TPCUDA_Alloc_Get_Free_Str(), TP_EXIT);
		TPCross_CheckError(TPCROSS_TEST_BytesToHex(), TP_EXIT);

		TPCross_CheckError(TPCROSS_TEST_HexVectorize(), TP_EXIT);

		TPCross_CheckError(TPCROSS_TEST_AddCDF(), TP_EXIT);
	}
	// #--------- Main ---------# //
	if(EnableCudaUnitTesting == 0)
	{
	}
	CloseCrossDatabase();
	exit(0);
}

