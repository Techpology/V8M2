#include <cuda_runtime.h>

#ifdef __cplusplus
extern "C"
{
	#include "../Errors/I_Cross_Errors.h"
	#include "../TP_Cross_Global.h"
}
#endif

#include "I_Cross_Cuda.hu"
#include "I_K_Cross.hu"

__global__ void TPCross_K_BytesToHex(char *_Str, size_t _StrSize, char *_Hex)
{
	size_t _id = threadIdx.x + blockIdx.x * blockDim.x;

	size_t StartIndex = _id * _BytesToHex_ChunkSize;
	size_t EndIndex = StartIndex + _BytesToHex_ChunkSize;
	EndIndex = (EndIndex > _StrSize) ? _StrSize : EndIndex;

	for (size_t i = StartIndex; i < EndIndex; i++)
	{
		_Hex[(i * 2)]		= _BytesToHex_HexChars[(_Str[i] >> 4) & 0xF];
		_Hex[(i * 2) + 1]	= _BytesToHex_HexChars[(_Str[i])	  & 0xF];
	}
}

__device__ char Hex00_FF[256][3] =
{
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f",
	"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f",
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
	"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f",
	"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4a", "4b", "4c", "4d", "4e", "4f",
	"50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
	"60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b", "6c", "6d", "6e", "6f",
	"70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d", "7e", "7f",
	"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
	"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f",
	"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af",
	"b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
	"c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf",
	"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "da", "db", "dc", "dd", "de", "df",
	"e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
	"f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff",
};

__device__ int TPCROSS_K_D_GetHexIndex(char *_Str, int _index)
{
	int res = -1;
	for(int i = 0; i < 256; i++)
	{
		if(Hex00_FF[i][0] == _Str[_index] && Hex00_FF[i][1] == _Str[_index + 1])
		{
			res = i;
			break;
		}
	}
	return res;
}

__global__ void TPCross_K_HexVectorize(char *_Str, size_t _StrSize, int *_Vector)
{
	// chunk
	// for hex in str
	//		find index
	//		Increment value at index
	size_t _id = threadIdx.x + blockIdx.x * blockDim.x;
	// size_t StrSize = (int)(_StrSize / 2);

	size_t StartIndex = _id * _HexVectorize_ChunkSize;
	size_t EndIndex = StartIndex + _HexVectorize_ChunkSize;
	EndIndex = (EndIndex > _StrSize) ? _StrSize : EndIndex;

	for (int i = StartIndex; i < EndIndex; i++)
	{
		int TargetIndex = TPCROSS_K_D_GetHexIndex(_Str, (i * 2));
		if(TargetIndex >= 0)
		{
			atomicAdd(&(_Vector[TargetIndex]), 1);
		}
		else{printf("ERROR:_K_HexVectorize()\n"); return;}
	}
}

__global__ void TPCross_K_HexIndex(char *_Str, size_t _StrSize, int *_IndexTable, int *_IndexTable_ActiveIndex, int *_IndexTable_StartIndex)
{
	size_t _id = threadIdx.x + blockIdx.x * blockDim.x;

	size_t StartIndex = _id * _HexIndex_ChunkSize;
	size_t EndIndex = StartIndex + _HexIndex_ChunkSize;
	EndIndex = (EndIndex > _StrSize) ? _StrSize : EndIndex;

	for (int i = StartIndex; i < EndIndex; i++)
	{
		int TargetIndex = TPCROSS_K_D_GetHexIndex(_Str, (i * 2));
		_IndexTable[_IndexTable_StartIndex[TargetIndex] + _IndexTable_ActiveIndex[TargetIndex]] = i;
		atomicAdd(&(_IndexTable_ActiveIndex[TargetIndex]), 1);
	}
}

__device__ int TPCross_K_D_FindSubstring(
	char *_String, size_t _String_size,
	char *_SubString, size_t _SubString_size, size_t _SubString_start,
	int *_String_index_table, int *_String_start_offset, int *_String_vector)
{
	int activeHex = TPCROSS_K_D_GetHexIndex(_SubString, _SubString_start);

	int ToRet = -1;
	for (int i = 0; i < _String_vector[activeHex]; i++)
	{
		int activePin = _String_index_table[_String_start_offset[activeHex] + i];
		int found = 1;
		for (int j = 0; j < _SubString_size; j++)
		{
			int _j = (j * 2);
			if(_SubString[_SubString_start + _j] != _String[activePin + _j] || _SubString[_SubString_start + (_j + 1)] != _String[activePin + _j + 1])
			{
				found = 0;
			}
		}

		if(found == 1)
		{
			ToRet = activePin;
			break;
		}
	}
	return ToRet;
}

__global__ void TPCross_K(
	char *_source, size_t _source_size, 
	char *_target, size_t _target_size, 
	int *_source_index_table, int *_source_start_offset, int *_source_vector,
	TP_CROSS_ReferenceObj *_result, int *resultSize)
{
	size_t _id = threadIdx.x + blockIdx.x * blockDim.x;

	size_t StartIndex = _id * _Cross_ChunkSize;
	size_t EndIndex = StartIndex + _Cross_ChunkSize;
	EndIndex = (EndIndex > _target_size) ? _target_size : EndIndex;

	size_t _res_index = _Cross_Res_Chunk * _id;
	size_t _res_startIndex = 0;
	size_t _res_endIndex = _Cross_Min_Reference;
	_res_endIndex = (_res_endIndex >= (EndIndex - StartIndex)) ? (EndIndex - StartIndex) : _Cross_Min_Reference;

	int queryCache = -1;
	int _found = TP_FALSE;
	for (int i = StartIndex; i < EndIndex;)
	{
		//printf("s%d", i); printf("l%d\n", _res_endIndex);
		int query = TPCross_K_D_FindSubstring(_source, _source_size, _target, _res_endIndex, i * 2, _source_index_table, _source_start_offset, _source_vector);
		//printf("qrr: %d\n", query);

		if(query == -1 || i > EndIndex - 1 || i + _res_endIndex > EndIndex || _res_endIndex > EndIndex)
		{
			if(_found == TP_TRUE)
			{
				if(_result[_res_index].isReference == 0 && _result[_res_index].endIndex > 0){ _res_index++; }
				_result[_res_index].startIndex = queryCache;
				_result[_res_index].endIndex = _res_endIndex - 1;
				_result[_res_index].isReference = 1;

				i += _res_endIndex - 1;
				
				if(i + _Cross_Min_Reference > EndIndex)
				{
					_res_endIndex = EndIndex - i; //printf("hhh: %d\n", EndIndex - i);
				}
				else { _res_endIndex = _Cross_Min_Reference; }

				_res_index++;
			}
			else
			{
				if(_result[_res_index].startIndex == 0 && _result[_res_index].endIndex == 0){ _result[_res_index].startIndex = i; }
				_result[_res_index].endIndex += 1;
				_result[_res_index].isReference = 0;
				i++;

				if(i + _Cross_Min_Reference > EndIndex)
				{
					_res_endIndex = EndIndex - i;
				}
				else { _res_endIndex = _Cross_Min_Reference; }
				if(i >= EndIndex || i + _res_endIndex > EndIndex) { _res_index++; }
			}
			_found = TP_FALSE;
		}
		else
		{
			_found = TP_TRUE;
			queryCache = query;
			_res_endIndex++;
		}
	}

	atomicAdd(resultSize, _res_index - (_Cross_Res_Chunk * _id)); // Total result size from this thread.
}


/* if(query != -1 && i != EndIndex && _res_endIndex <= EndIndex)
{
	_res_startIndex = query;
	_res_endIndex++;
	_found = TP_TRUE;
}
else
{
	if(_found == TP_FALSE)
	{
		_result[_res_index].endIndex += 1;
		i++;
	}
	else
	{
		if(_result[_res_index].isReference == 0 && _result[_res_index].endIndex > 0)
		{
			_res_index++;
		}
		_result[_res_index].endIndex = _res_endIndex;
		_result[_res_index].startIndex = i;
		_result[_res_index].isReference = 1;
		_res_index++;
		_found = TP_FALSE;
		i += _res_endIndex;
	}
} */
