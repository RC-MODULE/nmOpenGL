#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"


#define CHECK_STATUS(a) while (!msdGetStatusCopy(a))


SECTION(".text_demo3d") int getAddrPtrnsT(NMGL_Context_NM1* context, Patterns* patterns, Polygons* poly){
	Polygons* polyTmp = (Polygons*)context->buffer0;
	int size = poly->count;
	nm32s* temp0 = nmppsAddr_32s(context->buffer0, sizeof32(Polygons));
	nm32s* dydx = nmppsAddr_32s(temp0, NMGL_SIZE);
	msdAdd(poly->dy01, polyTmp->dy01, poly->count);										//0
	msdAdd(poly->dy12, polyTmp->dy12, poly->count);										//1
	msdAdd(poly->dy02, polyTmp->dy02, poly->count);										//2
	msdAdd(poly->x0, polyTmp->x0, poly->count);											//3
	msdAdd(poly->minX, polyTmp->minX, poly->count);										//4
	msdAdd(poly->dx01, polyTmp->dx01, poly->count);										//5
	msdAdd(poly->crossProduct, polyTmp->crossProduct, poly->count);						//6
	msdAdd(patterns->table_dydx, dydx, (2 * WIDTH_PTRN) * (2 * HEIGHT_PTRN + 2));		//7
	msdAdd(poly->dx12, polyTmp->dx12, poly->count);										//8
	msdAdd(poly->dx02, polyTmp->dx02, poly->count);										//9
	msdAdd(poly->maxDiffX, polyTmp->maxDiffX, poly->count);								//10
	msdAdd(poly->y0, polyTmp->y0, poly->count);											//11
	msdAdd(poly->color, polyTmp->color, 4 * poly->count);								//12
	msdAdd(poly->z, polyTmp->z, poly->count);											//13
	msdStartCopy();

	nm32s* temp2 = nmppsAddr_32s(dydx, (2 * WIDTH_PTRN) * (2 * HEIGHT_PTRN + 2));
	nm32s* x0C = (nm32s*)context->buffer1;
	nm32s* x0B = (nm32s*)nmppsAddr_32s(context->buffer1, SIZE_BANK / 2);
	nm32s* offPtrnSide = nmppsAddr_32s(x0C, NMGL_SIZE);
	nm32s* temp1 = nmppsAddr_32s(offPtrnSide, NMGL_SIZE);
	nm32s* temp3 = nmppsAddr_32s(x0B, NMGL_SIZE);
	nm2s* ptrnsRight_2s = (nm2s*)patterns->ptrns;
	nm2s* ptrnsLeft_2s = nmppsAddr_2s((nm2s*)patterns->ptrns, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN / 2);

	CHECK_STATUS(0);
	int height = size / SMALL_SIZE;
	int tail = size % SMALL_SIZE;
	int startTail = size / SMALL_SIZE;
	startTail *= SMALL_SIZE;

	nmppsMulC_32s(polyTmp->dy01, WIDTH_PTRN / 16, (nm32s*)temp0, size);
#ifdef __NM__
	nmppmCopy_32s((nm32s*)context->ppPtrns2_2s, 0, (nm32s*)temp1, SMALL_SIZE, height, SMALL_SIZE);
	nmppsCopy_32s((nm32s*)context->ppPtrns2_2s, temp1 + startTail, tail);
	nmppsAdd_32s((nm32s*)temp1, (nm32s*)temp0, (nm32s*)temp3, size);
#else
	for (int k = 0; k < size; k++) {
		temp3[k] = ((nm32s)context->ppPtrns2_2s[k % SMALL_SIZE] + temp0[k]);
	}
#endif

	int dstStride = 3 * SMALL_SIZE;
	nmppmCopy_32s((nm32s*)context->ppPtrns1_2s, 0, (nm32s*)context->ppDstPackPtrns, dstStride, height, SMALL_SIZE);
	nmppmCopy_32s((nm32s*)context->ppPtrns2_2s, 0, (nm32s*)context->ppDstPackPtrns + SMALL_SIZE, dstStride, height, SMALL_SIZE);
	nmppmCopy_32s((nm32s*)temp3, SMALL_SIZE, (nm32s*)context->ppDstPackPtrns + 2 * SMALL_SIZE, dstStride, height, SMALL_SIZE);

	int* dstTail = nmppsAddr_32s((nm32s*)context->ppDstPackPtrns, 3 * startTail);
	int* srcTail12 = nmppsAddr_32s((nm32s*)temp3, startTail);
	nmppsCopy_32s((nm32s*)context->ppPtrns1_2s, dstTail, tail);
	nmppsCopy_32s((nm32s*)context->ppPtrns2_2s, dstTail + tail, tail);
	nmppsCopy_32s((nm32s*)srcTail12, dstTail + 2 * tail, tail);

	CHECK_STATUS(1);
	nmppsMulC_32s(polyTmp->dy12, WIDTH_PTRN / 16, (nm32s*)temp1, size);
	CHECK_STATUS(2);
	nmppsMulC_32s(polyTmp->dy02, WIDTH_PTRN / 16, (nm32s*)temp2, size);
	mergePtrnsAddr3((nm32s**)temp2, (nm32s**)temp0, (nm32s**)temp1, SMALL_SIZE, (nm32s**)context->nSizePtrn32, size);

	CHECK_STATUS(3);
	CHECK_STATUS(4);
	nmppsSub_32s(polyTmp->x0, polyTmp->minX, (nm32s*)x0C, size);

	CHECK_STATUS(5);
	nmppsAdd_32s(x0C, polyTmp->dx01, x0B, size);

	CHECK_STATUS(6);
	selectPaintSide(polyTmp->crossProduct, (int)ptrnsLeft_2s, (int)ptrnsRight_2s, offPtrnSide, size);
	CHECK_STATUS(7);
	selectPatterns(dydx, polyTmp->dx01,
		polyTmp->dy01,
		x0C,
		offPtrnSide,
		(nm32s**)temp1, size, temp3);

	CHECK_STATUS(8);
	selectPatterns(dydx, polyTmp->dx12,
		polyTmp->dy12,
		x0B,
		offPtrnSide,
		(nm32s**)temp2, size, temp3);

	selectPaintSide(polyTmp->crossProduct, (int)ptrnsRight_2s, (int)ptrnsLeft_2s, offPtrnSide, size);
	CHECK_STATUS(9);
	selectPatterns(dydx, polyTmp->dx02,
		polyTmp->dy02,
		x0C,
		offPtrnSide,
		(nm32s**)temp0, size, temp3);

	mergePtrnsAddr3((nm32s**)temp0, (nm32s**)temp1, (nm32s**)temp2, SMALL_SIZE, context->ppSrcPackPtrns, size);

	CHECK_STATUS(10);
	nmppsAdd_32s(polyTmp->minX, polyTmp->maxDiffX, temp3, size);
	absIfNegElse0(polyTmp->minX, context->offsetTrX, size);
	nmppsClipCC_32s(polyTmp->minX, 0, poly->segWidth, polyTmp->minX, size);
	nmppsClipCC_32s(temp3, 0, poly->segWidth, temp3, size);
	nmppsSub_32s(temp3, polyTmp->minX, context->widths, size);

	
	
	nmppsAndC_32u((nm32u*)polyTmp->minX, 1, (nm32u*)temp1, size);
	nmppsSub_32s(polyTmp->minX, temp1, polyTmp->minX, size);
	nmppsSub_32s(context->offsetTrX, temp1, context->offsetTrX, size);
	nmppsAdd_32s(context->widths, temp1, context->widths, size);

	nmppsAndC_32u((nm32u*)context->widths, 1, (nm32u*)temp0, size);
	nmppsAdd_32s(context->widths, temp0, context->widths, size);

	CHECK_STATUS(11);
	nmppsAdd_32s(polyTmp->y0, polyTmp->dy02, temp1, size);		//maxY
	absIfNegElse0(polyTmp->y0, temp0, size);
	nmppsMulC_32s(temp0, WIDTH_PTRN / 16, context->offsetTrY, size);
	nmppsClipCC_32s(polyTmp->y0, 0, poly->segHeight, temp0, size);
	nmppsClipCC_32s(temp1, 0, poly->segHeight, temp3, size);
	nmppsSub_32s(temp3, temp0, context->heights, size);
	//temp0 - y0

	//расстановка точек вставок паттернов в зависимости от minX, y0
	nmppsMulC_AddV_AddC_32s(temp0,
		WIDTH_SEG,
		polyTmp->minX,
		(int)context->colorSegment.data,
		(nm32s*)context->imagePoints,
		size);

	CHECK_STATUS(12);
	nmppsConvert_32s8s(polyTmp->color, (nm8s*)context->valuesC, 4 * size);

	CHECK_STATUS(13);
	if (context->depthBuffer->enabled) {
		nmppsCopy_32s(polyTmp->z, context->valuesZ, size);
		nmppsMulC_AddV_AddC_32s(temp0,
			WIDTH_SEG,
			polyTmp->minX,
			(int)context->depthSegment.data,
			(nm32s*)context->zBuffPoints,
			size);
	}
	return 0;
}