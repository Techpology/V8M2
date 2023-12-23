#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../external/TPDBv2/src/Errors/I_Errors.h"
#include "../../external/TPDBv2/src/TPDB_Global.h"
#include "../../external/TPDBv2/src/Utils/MiscTools.h"
#include "../../external/TPDBv2/src/Interface/I_Row.h"
#include "../../external/TPDBv2/src/Interface/I_Database.h"
#include "../../external/TPDBv2/src/Interface/I_Table.h"

#include "../TP_Cross_Global.h"
#include "DatabaseManager.h"
#include "../GpuAccelerated/I_Cross_Cuda.hu"
#include "../Utils/CrossSerializers.h"
#include "../Utils/MiscTools.h"

TPDatabase	*CrossDataBase;
TPTable		*CDFtbl;
TPTable		*CVFtbl;
int **CVF_Vals_Global;

enum TPCROSS_ERROR_TYPES CreateCrossDatabase()
{
	CrossDataBase = CreateTPDatabase("CrossDB", "./db");
	AddTable(CrossDataBase, "CDF");
	AddTable(CrossDataBase, "CVF");

	CDFtbl = CrossDataBase->Tables[0];
	CVFtbl = CrossDataBase->Tables[1];

	SetColumnTypes(CDFtbl, 1, TP_STRING);

	SetColumnTypes(CVFtbl, 256, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT);
	CVFtbl->ColumnsIndexOffset = 1000;
	for (int i = 0; i < 256; i++)
	{
		TP_CheckError(AddIndexColumn(CVFtbl, i), TP_EXIT);
	}

	CDFtbl->RowsOnDemand = TP_TRUE;
	CVFtbl->RowsOnDemand = TP_FALSE;
	return TPCROSS_SUCCESS;
}

enum TPCROSS_ERROR_TYPES  GetCrossDatabase()
{
	CrossDataBase = CreateTPDatabase("CrossDB", "./db");
	GetTable(CrossDataBase, "CDF");
	GetTable(CrossDataBase, "CVF");

	CDFtbl = CrossDataBase->Tables[0];
	CVFtbl = CrossDataBase->Tables[1];

	SetColumnTypes(CDFtbl, 1, TP_STRING);

	SetColumnTypes(CVFtbl, 256, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT, TP_INT);
	CVFtbl->ColumnsIndexOffset = 1000;
	for (int i = 0; i < 256; i++)
	{
		TP_CheckError(AddIndexColumn(CVFtbl, i), TP_EXIT);
	}

	return TPCROSS_SUCCESS;
}

void CloseCrossDatabase()
{
	SyncTable(CDFtbl);
	SyncTable(CVFtbl);
	DestroyTPDatabase(CrossDataBase);
}

enum TPCROSS_ERROR_TYPES AddCDF(char *_Val)
{
	TP_CheckError(AddRow(CDFtbl, 1, _Val), TP_EXIT);

	int *ValVector = TPCross_HexVectorize(_Val, strlen(_Val));
 	
	AddRow(CVFtbl, 256, ValVector[0], ValVector[1], ValVector[2], ValVector[3], ValVector[4], ValVector[5], ValVector[6], ValVector[7], ValVector[8], ValVector[9], ValVector[10], ValVector[11], ValVector[12], ValVector[13], ValVector[14], ValVector[15], ValVector[16], ValVector[17], ValVector[18], ValVector[19], ValVector[20], ValVector[21], ValVector[22], ValVector[23], ValVector[24], ValVector[25], ValVector[26], ValVector[27], ValVector[28], ValVector[29], ValVector[30], ValVector[31], ValVector[32], ValVector[33], ValVector[34], ValVector[35], ValVector[36], ValVector[37], ValVector[38], ValVector[39], ValVector[40], ValVector[41], ValVector[42], ValVector[43], ValVector[44], ValVector[45], ValVector[46], ValVector[47], ValVector[48], ValVector[49], ValVector[50], ValVector[51], ValVector[52], ValVector[53], ValVector[54], ValVector[55], ValVector[56], ValVector[57], ValVector[58], ValVector[59], ValVector[60], ValVector[61], ValVector[62], ValVector[63], ValVector[64], ValVector[65], ValVector[66], ValVector[67], ValVector[68], ValVector[69], ValVector[70], ValVector[71], ValVector[72], ValVector[73], ValVector[74], ValVector[75], ValVector[76], ValVector[77], ValVector[78], ValVector[79], ValVector[80], ValVector[81], ValVector[82], ValVector[83], ValVector[84], ValVector[85], ValVector[86], ValVector[87], ValVector[88], ValVector[89], ValVector[90], ValVector[91], ValVector[92], ValVector[93], ValVector[94], ValVector[95], ValVector[96], ValVector[97], ValVector[98], ValVector[99], ValVector[100], ValVector[101], ValVector[102], ValVector[103], ValVector[104], ValVector[105], ValVector[106], ValVector[107], ValVector[108], ValVector[109], ValVector[110], ValVector[111], ValVector[112], ValVector[113], ValVector[114], ValVector[115], ValVector[116], ValVector[117], ValVector[118], ValVector[119], ValVector[120], ValVector[121], ValVector[122], ValVector[123], ValVector[124], ValVector[125], ValVector[126], ValVector[127], ValVector[128], ValVector[129], ValVector[130], ValVector[131], ValVector[132], ValVector[133], ValVector[134], ValVector[135], ValVector[136], ValVector[137], ValVector[138], ValVector[139], ValVector[140], ValVector[141], ValVector[142], ValVector[143], ValVector[144], ValVector[145], ValVector[146], ValVector[147], ValVector[148], ValVector[149], ValVector[150], ValVector[151], ValVector[152], ValVector[153], ValVector[154], ValVector[155], ValVector[156], ValVector[157], ValVector[158], ValVector[159], ValVector[160], ValVector[161], ValVector[162], ValVector[163], ValVector[164], ValVector[165], ValVector[166], ValVector[167], ValVector[168], ValVector[169], ValVector[170], ValVector[171], ValVector[172], ValVector[173], ValVector[174], ValVector[175], ValVector[176], ValVector[177], ValVector[178], ValVector[179], ValVector[180], ValVector[181], ValVector[182], ValVector[183], ValVector[184], ValVector[185], ValVector[186], ValVector[187], ValVector[188], ValVector[189], ValVector[190], ValVector[191], ValVector[192], ValVector[193], ValVector[194], ValVector[195], ValVector[196], ValVector[197], ValVector[198], ValVector[199], ValVector[200], ValVector[201], ValVector[202], ValVector[203], ValVector[204], ValVector[205], ValVector[206], ValVector[207], ValVector[208], ValVector[209], ValVector[210], ValVector[211], ValVector[212], ValVector[213], ValVector[214], ValVector[215], ValVector[216], ValVector[217], ValVector[218], ValVector[219], ValVector[220], ValVector[221], ValVector[222], ValVector[223], ValVector[224], ValVector[225], ValVector[226], ValVector[227], ValVector[228], ValVector[229], ValVector[230], ValVector[231], ValVector[232], ValVector[233], ValVector[234], ValVector[235], ValVector[236], ValVector[237], ValVector[238], ValVector[239], ValVector[240], ValVector[241], ValVector[242], ValVector[243], ValVector[244], ValVector[245], ValVector[246], ValVector[247], ValVector[248], ValVector[249], ValVector[250], ValVector[251], ValVector[252], ValVector[253], ValVector[254], ValVector[255]);

	free(ValVector); ValVector = NULL;
	return TPCROSS_SUCCESS;
}

void ParseCVF()
{
	if(CVFtbl->RowCount != NULL)
	{
		CVF_Vals_Global = (int**)malloc(sizeof(int*) * CVFtbl->RowCount);
		for (int i = 0; i < CVFtbl->RowCount; i++)
		{
			TPTable_Row *temp = CVFtbl->Rows[i];
			
			CVF_Vals_Global[i] = (int*)malloc(sizeof(int) * CVFtbl->ColCount);
			for (int j = 0; j < CVFtbl->ColCount; j++)
			{
				CVF_Vals_Global[i][j] = GetRowValue(CVFtbl, temp, j);
			}
		}
	}
}

int Search_ToCompress(char *_hex, size_t _hexSize, int* _vector)
{
	int MAX_Val = 0;
	int MAX_Ind = 0;
	for (int i = 0; i < 256; i++)
	{
		if(_vector[i] > MAX_Val)
		{
			MAX_Val = _vector[i];
			MAX_Ind = i;
		}
	}

	size_t querySize = 0;
	int *query = TP_GetIndexAtRange(CVFtbl, MAX_Ind, MAX_Val, &querySize);

	double bestCosResult = 0;
	int bestCosIndex = -1;
	for (int i = 0; i < querySize; i++)
	{
		double temp = cosineSimilarity(CVF_Vals_Global[query[i]], _vector, (unsigned int)256);
		if(temp > bestCosResult){ bestCosResult = temp; bestCosIndex = i; }
	}

	free(query); query = NULL;
	if(bestCosResult >= SEARCH_MIN_SIM){ return bestCosIndex; }
	else{return -1;}
}
