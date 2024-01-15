#ifndef TP_CROSS_MISCTOOLS_H
#define TP_CROSS_MISCTOOLS_H

FILE_INCOMING *FolderListener(char *folderPath);
enum TP_INCOMING_FILE_TYPE FORK_ToCompress_ToDecompress(char* _name);
double cosineSimilarity(int* A, int* B, unsigned int len);
int *StartOffset_1DArr(int *_a, int _size);

TP_CROSS_ReferenceObj *TP_CROSS_TrimResults(TP_CROSS_ReferenceObj *_incoming, int _ThreadCount, int _resSize);
char **TP_CROSS_ResultToStringArray(TP_CROSS_ReferenceObj *_incoming, int _resSize, int _id, char *_hex);

#endif
