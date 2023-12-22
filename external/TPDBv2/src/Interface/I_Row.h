#ifndef I_ROW_H
#define I_ROW_H

#include "../TPDB_Global.h"

TPTable_Row *CreateTPTableRow(int __ID, TPTable *_Table);
void DestroyTPTableRow(TPTable_Row *_self);
void EmptyRowValues(TPTable_Row *_self);

/*
 * Returns the value from the row specified at the column
 * @param table, the table where the row is from
 * @param row, the row to fetch from
 * @param column, the column in the row to retrive
 * */
void *GetRowValue(TPTable *table,TPTable_Row *row, int column);

enum TP_ERROR_TYPES UpdateRow(TPTable *_parent, TPTable_Row *_self);

enum TP_ERROR_TYPES SetRowValues(TPTable *_parent, TPTable_Row *_self, int _count, ...);

#endif
