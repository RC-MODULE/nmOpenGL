#include "nmpp.h"
#include "demo3d_nm1.h"



void fillPattern(nm8s* pDstSource, int width, int height);
void create_tabl_dydx(nm8s* srcPatterns, int* dydx, int width, int height);

void hostCreatePatterns(Patterns* patterns){
	nm8s* temp_8s = nmppsMalloc_8s(NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	nm4s* temp_4s = nmppsMalloc_4s(NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsSet_8s((nm8s*)temp_8s,0, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	fillPattern(temp_8s, WIDTH_PTRN, HEIGHT_PTRN);
	create_tabl_dydx(temp_8s,patterns->table_dydx,WIDTH_PTRN, HEIGHT_PTRN);
	nmppsConvert_8s4s((nm8s*)temp_8s,(nm4s*)temp_4s,NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsConvert_4s2s((nm4s*)temp_4s,(nm2s*)patterns->ptrns,NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsFree(temp_4s);
	nmppsFree(temp_8s);
}