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
