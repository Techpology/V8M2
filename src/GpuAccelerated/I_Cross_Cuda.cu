#include <cuda_runtime.h>

#include "I_Cross_Cuda.hu"
#include "I_K_Cross.hu"
#include "Cross_Cuda_Utils.hu"

#ifdef __cplusplus
extern "C"
{
	#include "../Errors/I_Cross_Errors.h"
	#include "../TP_Cross_Global.h"
}
#endif

char *TPCross_BytesToHex(char *_Str, size_t _StrSize)
{
	int ThreadsNeeded	= 0;
	int BlocksNeeded	= 0;
	Array1D_Cuda_KernelMap(&ThreadsNeeded, &BlocksNeeded, _StrSize, _BytesToHex_ChunkSize);

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
	Array1D_Cuda_KernelMap(&ThreadsNeeded, &BlocksNeeded, StrSize, _BytesToHex_ChunkSize);

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
int *TPCross_HexIndex(char *_Str, size_t _StrSize, int *_Vector, int *_StartOffset, int _RETURN_CUDA_2DARRAY)
{
	// Technically, I could replace HexVectorize with this function and a reference pass within this function.
	// It would add some speed and remove some overhead. I already wrote tests for the function prior and I already
	// wrote some code using both. It makes the code more readable and it is slightly more ram efficient
	// in the case the file never reaches the sourcing phase, so I will choose to not join them.
	// Unless profiling proves otherwhise.
	size_t StrSize = _StrSize / 2;

	int ThreadsNeeded	= 0;
	int BlocksNeeded	= 0;
	Array1D_Cuda_KernelMap(&ThreadsNeeded, &BlocksNeeded, StrSize, _HexIndex_ChunkSize);

	char *_Device_Str = TPCUDA_Alloc_Str(_Str, _StrSize + 1);
	int *_Device_Res = TPCUDA_Alloc_IntArrayEmpty((size_t)StrSize);
	int *_Device_Index = TPCUDA_Alloc_IntArrayEmpty(256);

	int *_Device_StartOffset = TPCUDA_Alloc_IntArrayEmpty(256);
	cudaMemcpy(_Device_StartOffset, _StartOffset, sizeof(int) * 256, cudaMemcpyHostToDevice);

	TPCross_K_HexIndex<<<BlocksNeeded, ThreadsNeeded>>>(_Device_Str, StrSize, _Device_Res, _Device_Index, _Device_StartOffset);

	if(_RETURN_CUDA_2DARRAY == 0)
	{
		int *_host_Res = (int*)malloc(sizeof(int) * StrSize);
		cudaMemcpy(_host_Res, _Device_Res, sizeof(int) * StrSize, cudaMemcpyDeviceToHost);

		TPCUDA_Free_Str(_Device_Str); _Device_Str = NULL;
		TPCUDA_Free_IntArray(_Device_Index); _Device_Index = NULL;
		TPCUDA_Free_IntArray(_Device_Res); _Device_Res = NULL;
		TPCUDA_Free_IntArray(_Device_StartOffset); _Device_StartOffset = NULL;
		return _host_Res;
	}
	else
	{
		TPCUDA_Free_Str(_Device_Str); _Device_Str = NULL;
		TPCUDA_Free_IntArray(_Device_Index); _Device_Index = NULL;
		TPCUDA_Free_IntArray(_Device_StartOffset); _Device_StartOffset = NULL;
		return _Device_Res;
	}
}

TP_CROSS_ReferenceObj *TPCross_Cross
(char *_Source, size_t _SourceSize, char *_Target, size_t _TargetSize, int *_SourceIndexTable, int *_SourceStartOffset, int *_SourceVector, int *_returnSize, int *_retThreadCount)
{
	int ThreadsNeeded = 0;
	int BlocksNeeded = 0;
	Array1D_Cuda_KernelMap(&ThreadsNeeded, &BlocksNeeded, (_TargetSize / 2), _Cross_ChunkSize);
	(*_retThreadCount) = ThreadsNeeded;

	char *d_Source = TPCUDA_Alloc_Str(_Source, _SourceSize);
	char *d_Target = TPCUDA_Alloc_Str(_Target, _TargetSize);

	int  *d_sourceIndexTable = TPCUDA_Alloc_IntArrayEmpty(_SourceSize);
	cudaMemcpy(d_sourceIndexTable, _SourceIndexTable, sizeof(int) * _SourceSize, cudaMemcpyHostToDevice);
	
	int  *d_sourceStartOffset = TPCUDA_Alloc_IntArrayEmpty(256);
	cudaMemcpy(d_sourceStartOffset, _SourceStartOffset, sizeof(int) * 256, cudaMemcpyHostToDevice);

	int  *d_sourceVector = TPCUDA_Alloc_IntArrayEmpty(256);
	cudaMemcpy(d_sourceVector, _SourceVector, sizeof(int) * 256, cudaMemcpyHostToDevice);

	TP_CROSS_ReferenceObj *d_result;
	cudaMalloc(&d_result, sizeof(TP_CROSS_ReferenceObj) * (_Cross_Res_Chunk * ThreadsNeeded));
	cudaMemset(d_result, 0, sizeof(TP_CROSS_ReferenceObj) * (_Cross_Res_Chunk * ThreadsNeeded));

	int *d_retSize;
	cudaMalloc(&d_retSize, sizeof(int));

	TPCross_K<<<BlocksNeeded, ThreadsNeeded>>>
		(d_Source, _SourceSize/2, d_Target, _TargetSize/2, d_sourceIndexTable, d_sourceStartOffset, d_sourceVector, d_result, d_retSize);

	TP_CROSS_ReferenceObj *h_results = (TP_CROSS_ReferenceObj*)malloc(sizeof(TP_CROSS_ReferenceObj) * (_Cross_Res_Chunk * ThreadsNeeded));
	cudaMemcpy(h_results, d_result, sizeof(TP_CROSS_ReferenceObj) * (_Cross_Res_Chunk * ThreadsNeeded), cudaMemcpyDeviceToHost);

	int *retSize = (int*)malloc(sizeof(int));
	cudaMemcpy(retSize, d_retSize, sizeof(int), cudaMemcpyDeviceToHost);
	(*_returnSize) = *retSize;

	free(retSize); retSize = NULL;
	cudaFree(d_retSize);
	TPCUDA_Free_Str(d_Source);
	TPCUDA_Free_Str(d_Target);
	TPCUDA_Free_IntArray(d_sourceIndexTable);
	TPCUDA_Free_IntArray(d_sourceStartOffset);
	TPCUDA_Free_IntArray(d_sourceVector);
	cudaFree(d_result); d_result = NULL;

	return h_results;
}

void I_TPCUDA_Free_IntArray(int *_IntArr)
{
	TPCUDA_Free_IntArray(_IntArr);
}
