#ifndef SERIALIZETOOL_H
#define SERIALIZETOOL_H
#include "../TPDB_Global.h"

char *SERIALIZE_Int_Str(int a);
char *SERIALIZE_Float_Str(float a);
char *SERIALIZE_Char_Str(char a);
char *SERIALIZE_Fkey_Str(TPForeignKey *a);

int   *SERIALIZE_Str_Int(char *str);
float *SERIALIZE_Str_Float(char *str);
/** Returns a pointer to a row retrived from the forienkey */
TPTable_Row *SERIALIZE_Str_Row(TPDatabase *database,char *str);

char *SERIALIZE_RowID_Path(TPTable *_tbl, int _id);

#endif
