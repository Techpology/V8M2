#ifndef TP_CROSS_MISCTOOLS_H
#define TP_CROSS_MISCTOOLS_H

FILE_INCOMING *FolderListener(char *folderPath);
enum TP_INCOMING_FILE_TYPE FORK_ToCompress_ToDecompress(char* _name);
double cosineSimilarity(int* A, int* B, unsigned int len);

#endif
