#include "nmpp.h"
#include "demo3d_nm1.h"



void fillPattern(nm8s* pDstSource, int width, int height);
void create_tabl_dydx(nm8s* srcPatterns, int* dydx, int width, int height);

void hostCreatePatterns(PatternsArray* patterns){
	nm8s* temp_8s = nmppsMalloc_8s(MAX(NPATTERNS, LINE_PATTERNS_AMOUNT)*WIDTH_PTRN*HEIGHT_PTRN);
	nm4s* temp_4s = nmppsMalloc_4s(MAX(NPATTERNS, LINE_PATTERNS_AMOUNT)*WIDTH_PTRN*HEIGHT_PTRN);

	nmppsSet_8s((nm8s*)temp_8s,0, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	//fillPattern(temp_8s, WIDTH_PTRN, HEIGHT_PTRN);
	fillPtrnsInit((nm8u*)temp_8s, 1);
	create_tabl_dydx(temp_8s,patterns->table_dydx,WIDTH_PTRN, HEIGHT_PTRN);
	nmppsConvert_8s4s((nm8s*)temp_8s,(nm4s*)temp_4s,NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsConvert_4s2s((nm4s*)temp_4s,(nm2s*)patterns->ptrns,NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);

	pointPtrnsInit((nm8u*)temp_8s, 1);
	nmppsConvert_8s4s((nm8s*)temp_8s, (nm4s*)temp_4s, POINT_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsConvert_4s2s((nm4s*)temp_4s, (nm2s*)patterns->pointPtrns, POINT_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);
	//nmppsSet_8s((nm8s*)temp_8s, 0, LINE_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN);
	//fillLinePattern(temp_8s, WIDTH_PTRN, HEIGHT_PTRN);
	//nmppsConvert_8s4s((nm8s*)temp_8s, (nm4s*)temp_4s, LINE_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);
	//nmppsConvert_4s2s((nm4s*)temp_4s, (nm2s*)patterns->linePtrns, LINE_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsFree(temp_4s);
	nmppsFree(temp_8s);
}