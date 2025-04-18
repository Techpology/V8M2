#include <cuda_runtime.h>

#ifdef __cplusplus
	extern "C"
	{
		#include "../Errors/I_Cross_Errors.h"
		#include "../TP_Cross_Global.h"
	}
#endif
#include "I_Cross_Cuda.hu"
#include "Cross_Cuda_Utils.hu"

void Array1D_Cuda_KernelMap(int *_ThreadsNeeded, int *_BlocksNeeded, size_t _StrSize, int _chunkSize)
{
	int ThreadsNeeded = (int)ceil((double)_StrSize/_chunkSize);
	ThreadsNeeded = IfZero(ThreadsNeeded, 1);

	int BlocksNeeded = (int)ceil((double)ThreadsNeeded/_ThreadsPerBlock);
	BlocksNeeded = IfZero(BlocksNeeded, 1);

	ThreadsNeeded = (ThreadsNeeded > _ThreadsPerBlock) ? _ThreadsPerBlock : ThreadsNeeded;

	*(_ThreadsNeeded) = ThreadsNeeded;
	*(_BlocksNeeded) = BlocksNeeded;
}

char *TPCUDA_Alloc_Str(char *_Str, size_t _StrSize)
{
	char *ToRet = NULL;
	size_t ToRetSize = (sizeof(char) * (_StrSize + 1));

	cudaError_t _malloc_ERR = cudaMalloc(&ToRet, ToRetSize);
	if(ToRet == NULL)
	{TPCUDA_Free_Str(ToRet); return NULL; }

	cudaError_t _memcpy_ERR = cudaMemcpy(ToRet, _Str, ToRetSize, cudaMemcpyHostToDevice);
	if(ToRet == NULL)
	{TPCUDA_Free_Str(ToRet); return NULL; }

	// printf("_malloc_ERR: %s\n", cudaGetErrorString(_malloc_ERR));
	// printf("_memcpy_ERR: %s\n", cudaGetErrorString(_memcpy_ERR));

	return ToRet;
}

char *TPCUDA_Alloc_StrEmpty(size_t _StrSize)
{
	char *ToRet;
	size_t ToRetSize = (sizeof(char) * (_StrSize + 1));

	cudaMalloc(&ToRet, ToRetSize);
	cudaMemset(ToRet, ' ', ToRetSize);

	return ToRet;
}

char *TPCUDA_GetStr(char *_Str, size_t _StrSize)
{
	size_t ToRetSize = (sizeof(char) * (_StrSize + 1));
	char *ToRet = (char*)malloc(ToRetSize);

	cudaMemcpy(ToRet, _Str, ToRetSize, cudaMemcpyDeviceToHost);

	return ToRet;
}

// int *TPCUDA_Alloc_IntArray()

int *TPCUDA_Alloc_IntArrayEmpty(size_t _ElementsCount)
{
	int *ToRet;
	size_t ToRetSize = (sizeof(int) * _ElementsCount);

	cudaMalloc(&ToRet, ToRetSize);
	cudaMemset(ToRet, 0, ToRetSize);

	return ToRet;
}

int *TPCUDA_GetIntArray(int *_IntArray, size_t _ElementsCount)
{
	size_t ToRetSize = (sizeof(int) * _ElementsCount);
	int *ToRet = (int*)malloc(ToRetSize);

	cudaMemcpy(ToRet, _IntArray, ToRetSize, cudaMemcpyDeviceToHost);

	return ToRet;
}

int **TPCUDA_Alloc_2DIntArrayEmpty(size_t _ElementsCount)
{
	int **ToRet;
	size_t ToRetSize = (sizeof(int*) * _ElementsCount);

	cudaMalloc(&ToRet, ToRetSize);

	return ToRet;
}

int **TPCUDA_Get2DIntArray(int **_2DIntArray, size_t _ElementsCount)
{
	// This function doesn't work, and I don't plan on making one, its just here cause it feels right.
	// In my case, the 2d int arrays are rarely if ever uniform, meaning i need to know the size of each int*.
	// Therefore, this function doesn't deserve being generic.
	size_t ToRetSize = (sizeof(int*) * _ElementsCount);
	int **ToRet = (int**)malloc(ToRetSize);

	cudaMemcpy(ToRet, _2DIntArray, ToRetSize, cudaMemcpyDeviceToHost);

	return ToRet;
}

TPCross_HexIndexObj **TPCUDA_ALLOC_HexIndexObj(size_t _count)
{
	TPCross_HexIndexObj **toRet;
	cudaMalloc(&toRet, sizeof(TPCross_HexIndexObj*) * _count);
	return toRet;
}

void TPCUDA_Free_Str(char *_Str)
{
	cudaFree(_Str);
}

void TPCUDA_Free_IntArray(int *_IntArr)
{
	cudaFree(_IntArr);
}

void TPCUDA_Free_2DIntArray(int **_IntArr, size_t _IntArrSize)
{
	for (size_t i = 0; i < _IntArrSize; i++)
	{
		cudaFree(_IntArr[i]);
	}
	cudaFree(_IntArr);
}
