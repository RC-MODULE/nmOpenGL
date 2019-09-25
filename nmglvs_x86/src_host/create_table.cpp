#include "nmpp.h"
#include "demo3d_nm1.h"


void sum(nm8s* src, nm32s* temp, nm32s* accTemp, int nOffSets_X, int sizePtrn)
{
	nmppsConvert_8s32s((nm8s*)src,(nm32s*)temp,sizePtrn);
	for (int i = 0; i < sizePtrn; i++) {
		if (temp[i] != 1) {
			temp[i] = 0;
		}
	}
	nmppsMulC_32s((nm32s*)temp, nOffSets_X, (nm32s*)temp, sizePtrn);
	nmppsAdd_32s((nm32s*)accTemp,(nm32s*)temp,(nm32s*)accTemp,sizePtrn);

};


void create_tabl_dydx(nm8s* srcPatterns,int* dydx, int width, int height){
	int sizePtrn = width*height;
	int nPtrn = (width + height)/STEP_PTRN;
	nmppsSet_32s(dydx, 0, 4*width*height);

//---------------dydx--------------------
	int i;
	nm32s* accTemp = nmppsMalloc_32s(sizePtrn);
	nm32s* temp = nmppsMalloc_32s(sizePtrn);	
//высчитывание и запись в таблицу dydx смещения относительно начала паттернов
//----------первая-и-вторая-четверть------------------------------------  
	nmppsSet_32s((nm32s*)accTemp, width - 1,sizePtrn);							//сдвиг смещения паттернов до x0=0
	for (i=1;i<nPtrn;i++){
		nm8s* src = nmppsAddr_8s((nm8s*)srcPatterns,sizePtrn*(i*OFFSETS + width-1));
		sum(src, temp, accTemp, OFFSETS, sizePtrn);	
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			dydx[y*2*width + x] = accTemp[y*width + x];
	}

	for (int x = 0; x < width; x++) {
		//dydx[sizePtrn + x] = dydx[sizePtrn - width + x];
		dydx[x] = x;
	}

	nmppsSet_32s((nm32s*)accTemp, nPtrn*OFFSETS, sizePtrn);							//сдвиг смещения паттернов до x0=0
	for (i = 1; i<nPtrn; i++)
	{
		nm8s* src = nmppsAddr_8s((nm8s*)srcPatterns, sizePtrn*((i+ nPtrn)*OFFSETS));
		sum(src, temp, accTemp, OFFSETS, sizePtrn);
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			dydx[y * 2 * width + (x+ width)] = accTemp[y*width + x];
	}
	for (int x = width; x < 2 * width; x++) {
		//dydx[sizePtrn + x] = dydx[sizePtrn - width + x];
		dydx[x] = x;
	}
	nmppsFree(temp);
	nmppsFree(accTemp);
//--------------------------------------------------------------------------------------
}