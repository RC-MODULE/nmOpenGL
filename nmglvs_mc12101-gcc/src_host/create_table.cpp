#include "nmpp.h"
#include "demo3d_nm1.h"
#include <math.h>


void sum(nm8s* src, nm32s* temp, nm32s* accTemp, int nOffSets_X, int sizePtrn)
{
	nmppsConvert_8s32s((nm8s*)src,(nm32s*)temp,sizePtrn);
	nmppsMulC_32s((nm32s*)temp, nOffSets_X, (nm32s*)temp, sizePtrn);
	nmppsAdd_32s((nm32s*)accTemp,(nm32s*)temp,(nm32s*)accTemp,sizePtrn);

};


void create_tabl_dydx(nm8s* srcPatterns,int* dydx, int width, int height){
	int sizePtrn = width*height;
	int nPtrn = AMOUNT_ANGLES/2;
	nmppsSet_32s(dydx, 0, 2*width*height);

//---------------dydx--------------------
	//заполнение 0..31 строки
	int i;
	nm32s* accTemp = nmppsMalloc_32s(sizePtrn);
	nm32s* temp = nmppsMalloc_32s(sizePtrn);	
//высчитывание и запись в таблицу dydx смещения относительно начала паттернов
//----------первая-и-вторая-четверть------------------------------------  
	nmppsSet_32s((nm32s*)accTemp, width - 1,sizePtrn);							//сдвиг смещения паттернов до x0=0
	for (i=2;i<nPtrn;i++){
		nm8s* src = nmppsAddr_8s((nm8s*)srcPatterns,sizePtrn*(i*OFFSETS + width-1));
		sum(src, temp, accTemp, OFFSETS, sizePtrn);
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			dydx[y*2*width + x] = accTemp[y*width + x];
	}

	nmppsSet_32s((nm32s*)accTemp, nPtrn*OFFSETS, sizePtrn);							//сдвиг смещения паттернов до x0=0
	for (i = 2; i<nPtrn; i++)
	{
		nm8s* src = nmppsAddr_8s((nm8s*)srcPatterns, sizePtrn*((i+ nPtrn)*OFFSETS));
		sum(src, temp, accTemp, OFFSETS, sizePtrn);
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			dydx[y * 2 * width + (x+ width)] = accTemp[y*width + x];
	}

	//дублировнаие 31 строки в 32 строку
	for (int i = 0; i < 2 * width; i++) {
		dydx[32 * 2 * width + i] = dydx[31 * 2 * width + i];
	}
	nmppsFree(temp);
	nmppsFree(accTemp);
//--------------------------------------------------------------------------------------
}