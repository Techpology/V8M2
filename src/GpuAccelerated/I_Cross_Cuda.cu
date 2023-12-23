#include <cuda_runtime.h>

#include "../TP_Cross_Global.h"
#include "I_Cross_Cuda.hu"
#include "I_K_Cross.hu"
#include "Cross_Cuda_Utils.hu"

char *TPCross_BytesToHex(char *_Str, size_t _StrSize)
{
	int ThreadsNeeded	= 0;
	int BlocksNeeded	= 0;
	Array1D_Cuda_KernelMap(&ThreadsNeeded, &BlocksNeeded, _StrSize);

	char *_Device_Str = TPCUDA_Alloc_Str(_Str, _StrSize);
	char *_Device_Res = TPCUDA_Alloc_StrEmpty(_StrSize * 2);

	TPCross_K_BytesToHex<<<BlocksNeeded, ThreadsNeeded>>>(_Device_Str, _StrSize, _Device_Res);
	char *_Host_Res = TPCUDA_GetStr(_Device_Res, _StrSize * 2);
	_Host_Res[_StrSize * 2] = '\0';

	TPCUDA_Free_Str(_Device_Str); _Device_Str = NULL;
	TPCUDA_Free_Str(_Device_Res); _Device_Res = NULL;

	return _Host_Res;
}

int *TPCross_HexVectorize(char *_Str, size_t _StrSize)
{
	size_t StrSize = _StrSize / 2;

	int ThreadsNeeded	= 0;
	int BlocksNeeded	= 0;
	Array1D_Cuda_KernelMap(&ThreadsNeeded, &BlocksNeeded, StrSize);

	char *_Device_Str = TPCUDA_Alloc_Str(_Str, _StrSize + 1);
	int *_Device_Res = TPCUDA_Alloc_IntArrayEmpty((unsigned long)256);

	TPCross_K_HexVectorize<<<BlocksNeeded, ThreadsNeeded>>>(_Device_Str, StrSize, _Device_Res);

	int *_Host_Res = TPCUDA_GetIntArray(_Device_Res, 256);

	TPCUDA_Free_Str(_Device_Str); _Device_Str = NULL;
	TPCUDA_Free_IntArray(_Device_Res); _Device_Res = NULL;
	return _Host_Res;
}

/*
 * If {_RETURN_CUDA_2DARRAY} is set to 1, then a cuda int** will be returned.
 * Else, a C ready int** will be returned.
*/
int **TPCross_HexIndex(char *_Str, size_t _StrSize, int *_Vector, int _RETURN_CUDA_2DARRAY)
{
	// Technically, I could replace HexVectorize with this function and a reference pass within this function.
	// It would add some speed and remove some overhead. I already wrote tests for the function prior and I already
	// wrote some code using both. It makes the code more readable and it is slightly more ram efficient
	// in the case the file never reaches the sourcing phase, so I will choose to not join them.
	// Unless profiling proves otherwhise.
	size_t StrSize = _StrSize / 2;

	int ThreadsNeeded	= 0;
	int BlocksNeeded	= 0;
	Array1D_Cuda_KernelMap(&ThreadsNeeded, &BlocksNeeded, StrSize);

	char *_Device_Str = TPCUDA_Alloc_Str(_Str, _StrSize + 1);
	int **_Device_Res = TPCUDA_Alloc_2DIntArrayEmpty((size_t)256);
	for (int i = 0; i < 256; i++)
	{
		_Device_Res[i] = TPCUDA_Alloc_IntArrayEmpty(_Vector[i]);
	}
	int *_Device_Index = TPCUDA_Alloc_IntArrayEmpty(256);

	TPCross_K_HexIndex<<<BlocksNeeded, ThreadsNeeded>>>(_Device_Str, StrSize, _Device_Res, _Device_Index);

	if(_RETURN_CUDA_2DARRAY == 0)
	{
		int **_host_Res = (int**)malloc(sizeof(int*) * 256);
		for (int i = 0; i < 256; i++)
		{
			_host_Res[i] = TPCUDA_GetIntArray(_Device_Res[i], _Vector[i]);
		}
		TPCUDA_Free_Str(_Device_Str); _Device_Str = NULL;
		TPCUDA_Free_IntArray(_Device_Index); _Device_Res = NULL;
		TPCUDA_Free_2DIntArray(_Device_Res, 256);
		return _host_Res;
	}
	else
	{
		TPCUDA_Free_Str(_Device_Str); _Device_Str = NULL;
		TPCUDA_Free_IntArray(_Device_Index); _Device_Res = NULL;
		return _Device_Res;
	}
}

TP_CROSS_ReferenceObj *TPCross_Cross(char *_Source, size_t _SourceSize, char *_Target, size_t _TargetSize, int **_SourceIndexTable, int *_SourceVector)
{
	char *_Device_Source			= TPCUDA_Alloc_Str(_Source, _SourceSize);
	char *_Device_Target			= TPCUDA_Alloc_Str(_Target, _TargetSize);
	int **_Device_SourceIndexTable	= _SourceIndexTable;
	int  *_Device_SourceVector		= TPCUDA_Alloc_IntArrayEmpty(256);
	cudaMemcpy(_Device_SourceVector, _SourceVector, sizeof(int) * 256, cudaMemcpyHostToDevice);
	
}
