#include "hal.h"
#include "nmpp.h"
#include "pattern.h"

void hostCreatePatterns(PatternsArray* patterns){
	int size = NPATTERNS * WIDTH_PTRN * HEIGHT_PTRN;
	nm32u* temp0 = (nm32u*)halMalloc32(size);
	nm8s* temp1 = (nm8s*)halMalloc32(size / 4);

	nm32s* temp32s = (nm32s*)temp0;
	nm8s* temp8s = temp1;
	nm4s* temp4s = (nm4s*)temp0;

	fillPtrnsInit(temp0, patterns->table_dydx, 1);
	nmppsConvert_32s8s(temp32s, temp8s, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsConvert_8s4s(temp8s, temp4s, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsConvert_4s2s(temp4s, patterns->ptrns, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);

	pointPtrnsInit(temp0, 1);
	nmppsConvert_32s8s(temp32s, temp8s, POINT_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsConvert_8s4s(temp8s, temp4s, POINT_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsConvert_4s2s(temp4s, patterns->pointPtrns, POINT_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);

	linePtrnsInit(temp0, patterns->lineTable, 1);
	nmppsConvert_32s8s(temp32s, temp8s, LINE_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsConvert_8s4s(temp8s, temp4s, LINE_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsConvert_4s2s(temp4s, patterns->linePtrns, LINE_PATTERNS_AMOUNT*WIDTH_PTRN*HEIGHT_PTRN);

	halFree(temp0);
	halFree(temp1);
}