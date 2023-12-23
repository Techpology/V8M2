#ifndef TP_CROSS_DATABASE_MANAGER_H
#define TP_CROSS_DATABASE_MANAGER_H

#include "../Errors/I_Cross_Errors.h"

enum TPCROSS_ERROR_TYPES CreateCrossDatabase();
enum TPCROSS_ERROR_TYPES  GetCrossDatabase();
void CloseCrossDatabase();
void ParseCVF();

enum TPCROSS_ERROR_TYPES AddCDF(char *_Val);

int Search_ToCompress(char *_hex, size_t _hexSize, int* _vector);

#endif
