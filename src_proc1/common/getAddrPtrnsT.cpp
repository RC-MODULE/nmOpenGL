#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"


#define CHECK_STATUS(a) while (!msdGetStatusCopy(a))
//#define CHECK_STATUS(a) 


SECTION(".text_demo3d") int getAddrPtrnsT(NMGL_Context_NM1* context, Patterns* patterns, Polygons* poly){
	Polygons* polyTmp = (Polygons*)context->buffer0;
	int size = poly->count;
	nm32s* temp0 = nmppsAddr_32s(context->buffer0, sizeof32(Polygons));
	nm32s* dydx = nmppsAddr_32s(temp0, NMGL_SIZE);

	msdAdd(poly->crossProduct, polyTmp->crossProduct, size);
	msdAdd(poly->numbersPattrns01, polyTmp->numbersPattrns01, size);
	msdAdd(poly->numbersPattrns12, polyTmp->numbersPattrns12, size);
	msdAdd(poly->numbersPattrns02, polyTmp->numbersPattrns02, size);
	msdAdd(poly->pointInImage, polyTmp->pointInImage, size);
	msdAdd(poly->widths, polyTmp->widths, size);
	msdAdd(poly->offsetsX, polyTmp->offsetsX, size);
	msdAdd(poly->color, polyTmp->color, 4 * size);
	msdAdd(poly->ptrnSizesOf32_01, polyTmp->ptrnSizesOf32_01, size);
	msdAdd(poly->ptrnSizesOf32_12, polyTmp->ptrnSizesOf32_12, size);
	msdAdd(poly->ptrnSizesOf32_02, polyTmp->ptrnSizesOf32_02, size);

	msdAdd(poly->z, context->valuesZ, size);
	msdAdd(poly->offsetsY, context->offsetTrY, size);
	msdAdd(poly->heights, context->heights, size);
	msdStartCopy();

	nm32s* temp2 = nmppsAddr_32s(dydx, (2 * WIDTH_PTRN) * (2 * HEIGHT_PTRN + 2));
	nm32s* offPtrnSide = nmppsAddr_32s(temp2, NMGL_SIZE);
	nm32s* temp1 = nmppsAddr_32s(offPtrnSide, NMGL_SIZE);
	nm2s* ptrnsRight_2s = (nm2s*)patterns->ptrns;
	nm2s* ptrnsLeft_2s = nmppsAddr_2s((nm2s*)patterns->ptrns, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN / 2);

	CHECK_STATUS(0);
	selectPaintSide(polyTmp->crossProduct, (int)ptrnsLeft_2s, (int)ptrnsRight_2s, offPtrnSide, size);
	CHECK_STATUS(1);
	nmppsMulC_AddV_AddC_32s(polyTmp->numbersPattrns01, WIDTH_PTRN * HEIGHT_PTRN / 16, offPtrnSide, 0, temp1, size);
	CHECK_STATUS(2);
	nmppsMulC_AddV_AddC_32s(polyTmp->numbersPattrns12, WIDTH_PTRN * HEIGHT_PTRN / 16, offPtrnSide, 0, temp2, size);
	selectPaintSide(polyTmp->crossProduct, (int)ptrnsRight_2s, (int)ptrnsLeft_2s, offPtrnSide, size);
	CHECK_STATUS(3);
	nmppsMulC_AddV_AddC_32s(polyTmp->numbersPattrns02, WIDTH_PTRN * HEIGHT_PTRN / 16, offPtrnSide, 0, temp0, size);

	mergePtrnsAddr3((nm32s**)temp0, (nm32s**)temp1, (nm32s**)temp2, SMALL_SIZE, context->ppSrcPackPtrns, size);


	CHECK_STATUS(4);
	nmppsAndC_32u((nm32u*)polyTmp->pointInImage, 1, (nm32u*)temp1, size);
	nmppsSub_32s(polyTmp->pointInImage, temp1, polyTmp->pointInImage, size);
	CHECK_STATUS(5);
	nmppsAdd_32s(polyTmp->widths, temp1, temp2, size);
	nmppsAndC_32u((nm32u*)temp2, 1, (nm32u*)temp0, size);
	nmppsAdd_32s(temp2, temp0, context->widths, size);
	CHECK_STATUS(6);
	nmppsSub_32s(polyTmp->offsetsX, temp1, context->offsetTrX, size);

	//расстановка точек вставок паттернов в зависимости от minX, y0
	nmppsAddC_32s(polyTmp->pointInImage, (int)context->colorSegment.data, (nm32s*)context->imagePoints, size);
	nmppsAddC_32s(polyTmp->pointInImage, (int)context->depthSegment.data, (nm32s*)context->zBuffPoints, size);

	CHECK_STATUS(7);
	nmppsConvert_32s8s(polyTmp->color, (nm8s*)context->valuesC, 4 * size);

	int height = size / SMALL_SIZE;
	int tail = size % SMALL_SIZE;
	int startTail = size / SMALL_SIZE;
	startTail *= SMALL_SIZE;
	CHECK_STATUS(8);
	nmppmCopy_32s((nm32s*)context->ppPtrns2_2s, 0, (nm32s*)temp1, SMALL_SIZE, height, SMALL_SIZE);
	nmppsCopy_32s((nm32s*)context->ppPtrns2_2s, temp1 + startTail, tail);
	nmppsAdd_32s((nm32s*)temp1, (nm32s*)polyTmp->ptrnSizesOf32_01, (nm32s*)temp0, size);

	int dstStride = 3 * SMALL_SIZE;
	nmppmCopy_32s((nm32s*)context->ppPtrns1_2s, 0, (nm32s*)context->ppDstPackPtrns, dstStride, height, SMALL_SIZE);
	nmppmCopy_32s((nm32s*)context->ppPtrns2_2s, 0, (nm32s*)context->ppDstPackPtrns + SMALL_SIZE, dstStride, height, SMALL_SIZE);
	nmppmCopy_32s((nm32s*)temp0, SMALL_SIZE, (nm32s*)context->ppDstPackPtrns + 2 * SMALL_SIZE, dstStride, height, SMALL_SIZE);

	int* dstTail = nmppsAddr_32s((nm32s*)context->ppDstPackPtrns, 3 * startTail);
	int* srcTail12 = nmppsAddr_32s((nm32s*)temp0, startTail);
	nmppsCopy_32s((nm32s*)context->ppPtrns1_2s, dstTail, tail);
	nmppsCopy_32s((nm32s*)context->ppPtrns2_2s, dstTail + tail, tail);
	nmppsCopy_32s((nm32s*)srcTail12, dstTail + 2 * tail, tail);

	CHECK_STATUS(9);
	CHECK_STATUS(10);
	mergePtrnsAddr3((nm32s**)polyTmp->ptrnSizesOf32_02, 
		(nm32s**)polyTmp->ptrnSizesOf32_01, 
		(nm32s**)polyTmp->ptrnSizesOf32_12,
		SMALL_SIZE,
		(nm32s**)context->nSizePtrn32,
	 size);
	return 0;
}