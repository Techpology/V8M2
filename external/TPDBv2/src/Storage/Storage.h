#ifndef STORAGE_H
#define STORAGE_H

enum TP_ERROR_TYPES TP_Mkdir(char *_path);
enum TP_ERROR_TYPES TP_StoreFile(char *_path, char *_val);
char *TP_ReadFile(char* _path);

#endif