#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../TP_Cross_Global.h"
#include "../GpuAccelerated/I_Cross_Cuda.hu"
#include "../Database/DatabaseManager.h"
#include "../../external/TPDBv2/src/Utils/SerializeTools.h"
#include "../../external/TPDBv2/src/Utils/StringTools.h"
#include "../../external/TPDBv2/src/Storage/Storage.h"

FILE_INCOMING *ToCompress;
char	*ToCompress_Hex;
size_t	 ToCompress_Hex_size;
int		*ToCompress_Vector;

char *TP_CROSS_GetCompressed(FILE_INCOMING *_ToCompress)
{
	ToCompress = _ToCompress;
	ToCompress_Hex_size = ToCompress->_size * 2;

	ToCompress_Hex = TPCross_BytesToHex(ToCompress->_content, ToCompress->_size);
	ToCompress_Vector = TPCross_HexVectorize(ToCompress_Hex, ToCompress_Hex_size);

	CROSS(NULL, NULL);
}

char *TP_CROSS_SaveAndReturn(char *_hex, int *_vector)
{
	int toRet = CDFtbl->LastRowID;
	if(_hex == NULL && _vector == NULL)
	{
		_hex = ToCompress_Hex;
		_vector = ToCompress_Vector;
	}
	TP_CheckError(AddRow(CDFtbl, 1, _hex), TP_EXIT);
	TP_CheckError(AddRow(CVFtbl, 256
	, _vector[0], _vector[1], _vector[2], _vector[3], _vector[4], _vector[5], _vector[6], _vector[7], _vector[8], _vector[9], _vector[10], 
	_vector[11], _vector[12], _vector[13], _vector[14], _vector[15], _vector[16], _vector[17], _vector[18], _vector[19], _vector[20], 
	_vector[21], _vector[22], _vector[23], _vector[24], _vector[25], _vector[26], _vector[27], _vector[28], _vector[29], _vector[30], 
	_vector[31], _vector[32], _vector[33], _vector[34], _vector[35], _vector[36], _vector[37], _vector[38], _vector[39], _vector[40], 
	_vector[41], _vector[42], _vector[43], _vector[44], _vector[45], _vector[46], _vector[47], _vector[48], _vector[49], _vector[50], 
	_vector[51], _vector[52], _vector[53], _vector[54], _vector[55], _vector[56], _vector[57], _vector[58], _vector[59], _vector[60], 
	_vector[61], _vector[62], _vector[63], _vector[64], _vector[65], _vector[66], _vector[67], _vector[68], _vector[69], _vector[70], 
	_vector[71], _vector[72], _vector[73], _vector[74], _vector[75], _vector[76], _vector[77], _vector[78], _vector[79], _vector[80], 
	_vector[81], _vector[82], _vector[83], _vector[84], _vector[85], _vector[86], _vector[87], _vector[88], _vector[89], _vector[90], 
	_vector[91], _vector[92], _vector[93], _vector[94], _vector[95], _vector[96], _vector[97], _vector[98], _vector[99], _vector[100], 
	_vector[101], _vector[102], _vector[103], _vector[104], _vector[105], _vector[106], _vector[107], _vector[108], _vector[109], _vector[110], 
	_vector[111], _vector[112], _vector[113], _vector[114], _vector[115], _vector[116], _vector[117], _vector[118], _vector[119], _vector[120], 
	_vector[121], _vector[122], _vector[123], _vector[124], _vector[125], _vector[126], _vector[127], _vector[128], _vector[129], _vector[130], 
	_vector[131], _vector[132], _vector[133], _vector[134], _vector[135], _vector[136], _vector[137], _vector[138], _vector[139], _vector[140], 
	_vector[141], _vector[142], _vector[143], _vector[144], _vector[145], _vector[146], _vector[147], _vector[148], _vector[149], _vector[150], 
	_vector[151], _vector[152], _vector[153], _vector[154], _vector[155], _vector[156], _vector[157], _vector[158], _vector[159], _vector[160], 
	_vector[161], _vector[162], _vector[163], _vector[164], _vector[165], _vector[166], _vector[167], _vector[168], _vector[169], _vector[170], 
	_vector[171], _vector[172], _vector[173], _vector[174], _vector[175], _vector[176], _vector[177], _vector[178], _vector[179], _vector[180], 
	_vector[181], _vector[182], _vector[183], _vector[184], _vector[185], _vector[186], _vector[187], _vector[188], _vector[189], _vector[190], 
	_vector[191], _vector[192], _vector[193], _vector[194], _vector[195], _vector[196], _vector[197], _vector[198], _vector[199], _vector[200], 
	_vector[201], _vector[202], _vector[203], _vector[204], _vector[205], _vector[206], _vector[207], _vector[208], _vector[209], _vector[210], 
	_vector[211], _vector[212], _vector[213], _vector[214], _vector[215], _vector[216], _vector[217], _vector[218], _vector[219], _vector[220], 
	_vector[221], _vector[222], _vector[223], _vector[224], _vector[225], _vector[226], _vector[227], _vector[228], _vector[229], _vector[230], 
	_vector[231], _vector[232], _vector[233], _vector[234], _vector[235], _vector[236], _vector[237], _vector[238], _vector[239], _vector[240], 
	_vector[241], _vector[242], _vector[243], _vector[244], _vector[245], _vector[246], _vector[247], _vector[248], _vector[249], _vector[250], 
	_vector[251], _vector[252], _vector[253], _vector[254], _vector[255]), TP_EXIT);

	char *idString = SERIALIZE_Int_Str(toRet);
	char *toRetStr = TP_StrnCat("i", 1, idString);
	free(idString); idString = NULL;
	return toRetStr;
}

char *CROSS(char *_hex, int *_vector)
{
	if(_hex == NULL && _vector == NULL)
	{
		_hex	= ToCompress_Hex;
		_vector = ToCompress_Vector;
	}

	int QueryResult = Search_ToCompress(ToCompress_Hex, ToCompress_Hex_size, ToCompress_Vector);
	if(QueryResult == -1)
	{
		return TP_CROSS_SaveAndReturn(NULL, NULL);
	}

	char *Source_id_str			= SERIALIZE_Int_Str(QueryResult);
	char *Source_path			= TP_StrnCat(CDFtbl->Path, 2, "/", Source_id_str);
	free(Source_id_str);		  Source_id_str = NULL;

	char *Source_bytes			= TP_ReadFile(Source_path);
	size_t Source_bytes_size	= strlen(Source_bytes);
	char *Source_hex			= TPCross_BytesToHex(Source_bytes, Source_bytes_size);
	char *Source_hex_size		= Source_bytes_size * 2;

	int *Source_vector			= TPCross_HexVectorize(Source_hex, Source_hex_size);
	int **Source_Index			= TPCross_HexIndex(Source_hex, Source_hex_size, Source_vector, 1);

	// Reference
}

void FreeProcess_Compress()
{
	free(ToCompress_Hex); ToCompress_Hex = NULL;
	free(ToCompress_Vector); ToCompress_Vector = NULL;
}
