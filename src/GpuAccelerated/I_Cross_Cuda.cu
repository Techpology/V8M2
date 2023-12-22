#include <cuda_runtime.h>

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

