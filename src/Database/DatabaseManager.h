#ifndef TP_CROSS_DATABASE_MANAGER_H
#define TP_CROSS_DATABASE_MANAGER_H

#include "../Errors/I_Cross_Errors.h"

enum TPCROSS_ERROR_TYPES CreateCrossDatabase();
enum TPCROSS_ERROR_TYPES  GetCrossDatabase();
void CloseCrossDatabase();

enum TPCROSS_ERROR_TYPES AddCDF(char *_Val);

#endif
