#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/inotify.h>

//#include "../../external/TPDBv2/src/TPDB_Global.h"
#include "../TP_Cross_Global.h"
#include "../../external/TPDBv2/src/Utils/StringTools.h"
#include "../../external/TPDBv2/src/Storage/Storage.h"
#include "../../external/TPDBv2/src/Utils/MiscTools.h"
#include "MiscTools.h"
#include "CrossSerializers.h"
#include "../GpuAccelerated/I_Cross_Cuda.hu"

#define MAX_EVENTS 1024
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (MAX_EVENTS * (EVENT_SIZE + 16))

FILE_INCOMING *FolderListener(char *folderPath)
{
	int inotify_fd = inotify_init();
	int watch_descriptor = inotify_add_watch(inotify_fd, folderPath, IN_CREATE | IN_CLOSE_WRITE);

	FILE_INCOMING *ToReturn = (FILE_INCOMING*)malloc(sizeof(FILE_INCOMING));

	char buffer[BUF_LEN];
	int GoTo_Return = TP_FALSE;
	while(GoTo_Return == TP_FALSE)
	{
		ssize_t len = read(inotify_fd, buffer, BUF_LEN);

		for(char *ptr = buffer; ptr < buffer + len; ptr += EVENT_SIZE + ((struct inotify_event*) ptr)->len)
		{
			struct inotify_event *event = (struct inotify_event*)ptr;
			if(event->mask & IN_CLOSE_WRITE)
			{
				char *EntryPath = TP_StrnCat(folderPath, 1, event->name);
				ToReturn->_name = strdup(event->name);

				struct stat file_stat;
				if(stat(EntryPath, &file_stat) == 0)
				{
					off_t file_size = file_stat.st_size;
					ToReturn->_size = (size_t)file_size;
				}
				ToReturn->_content = TP_ReadFile(EntryPath);

				free(EntryPath); EntryPath = NULL;
				GoTo_Return = TP_TRUE;
			}
		}
	}

	inotify_rm_watch(inotify_fd, watch_descriptor);
	close(inotify_fd);
	return ToReturn;
}

enum TP_INCOMING_FILE_TYPE FORK_ToCompress_ToDecompress(char* _name)
{
	int slices = 0;
	char **NameSplit = TP_SplitString(_name, '.', &slices);

	if(strcmp(NameSplit[slices - 1], "ccf") == 0)
	{
		FreeArrayOfPointers((void***)&NameSplit, (size_t)slices);
		return TP_CROSS_FILE_COMPRESSED;
	}
	else
	{
		FreeArrayOfPointers((void***)&NameSplit, (size_t)slices);
		return TP_CROSS_FILE_ANY;
	}
}

double cosineSimilarity(int* A, int* B, unsigned int len)
{
	double dot = 0.0, denom_a = 0.0, denom_b = 0.0;
	for (size_t i = 0; i < len; i++)
	{
		dot += (double)A[i] * (double)B[i];
		denom_a += (double)A[i] * (double)A[i];
		denom_b += (double)B[i] * (double)B[i];
	}
		
	// Check for division by zero before performing the division
	if (denom_a == 0.0 || denom_b == 0.0)
	{
		return 0.0; // or some other suitable value based on your use case
	}
		
	return dot / (sqrt(denom_a) * sqrt(denom_b));
}

int *StartOffset_1DArr(int *_a, int _size)
{
	int *Ret = (int*)malloc(sizeof(int) * _size);
	int current = 0;
	for (int i = 0; i < _size; i++)
	{
		Ret[i] = (_a[i] > 0) ? current : -1;
		current += _a[i];
	}
	return Ret;
}

TP_CROSS_ReferenceObj *TP_CROSS_TrimResults(TP_CROSS_ReferenceObj *_incoming, int _ThreadCount, int _resSize)
{
	TP_CROSS_ReferenceObj *ToRet = (TP_CROSS_ReferenceObj*)malloc(sizeof(TP_CROSS_ReferenceObj) * _resSize);
	int activeIndex = 0;

	for (int i = 0; i < _ThreadCount; i++)
	{
		int rangeIndex = i * _Cross_Res_Chunk;
		for (int j = 0; j < _resSize; j++)
		{
			if(_incoming[rangeIndex + j].endIndex == 0){break;}
			ToRet[activeIndex].startIndex = _incoming[rangeIndex + j].startIndex;
			ToRet[activeIndex].endIndex = _incoming[rangeIndex + j].endIndex;
			ToRet[activeIndex].isReference = _incoming[rangeIndex + j].isReference;
			activeIndex++;
		}
	}
	return ToRet;
}

char **TP_CROSS_ResultToStringArray(TP_CROSS_ReferenceObj *_incoming, int _resSize, int _id, char *_hex)
{
	char **ToRet = (char**)malloc(sizeof(char*) * _resSize);

	for (int i = 0; i < _resSize; i++)
	{
		if(_incoming[i].isReference == 1)
		{
			ToRet[i] = CrossSerializer_Ref_Str(_id, _incoming[i].startIndex, _incoming[i].endIndex);
		}
		else
		{
			ToRet[i] = CrossSerializer_Inj_Str(_incoming[i].startIndex, _incoming[i].endIndex, _hex);
		}
	}
	return ToRet;
}
