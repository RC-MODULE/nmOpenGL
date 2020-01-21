#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"




SECTION(".data_demo3d") unsigned int points[14];

#define CHECK_STATUS(a) while (!msdGetStatusCopy(points[a]))


SECTION(".text_demo3d") int getAddrPtrnsT(NMGL_Context_NM1* context, Patterns* patterns, Polygons* poly){
	Polygons* polyTmp = (Polygons*)context->buffer0;
	int size = poly->count;
	nm32s* temp0 = nmppsAddr_32s(context->buffer0, sizeof32(Polygons));
	nm32s* dydx = nmppsAddr_32s(temp0, NMGL_SIZE);
	points[0] = msdAdd(poly->ptrnSizesOf32_01, polyTmp->ptrnSizesOf32_01, size);
	points[1] = msdAdd(poly->pointInImage, polyTmp->pointInImage, size);
	points[2] = msdAdd(poly->widths, polyTmp->widths, size);
	points[3] = msdAdd(poly->offsetsX, polyTmp->offsetsX, size);
	points[4] = msdAdd(poly->numbersPattrns01, polyTmp->numbersPattrns01, size);
	points[5] = msdAdd(poly->numbersPattrns12, polyTmp->numbersPattrns12, size);
	points[6] = msdAdd(poly->numbersPattrns02, polyTmp->numbersPattrns02, size);
	points[7] = msdAdd(poly->color, polyTmp->color, 4 * size);
	points[8] = msdAdd(poly->ptrnSizesOf32_12, polyTmp->ptrnSizesOf32_12, size);
	points[9] = msdAdd(poly->ptrnSizesOf32_02, polyTmp->ptrnSizesOf32_02, size);

	points[10] = msdAdd(poly->z, context->valuesZ, size);
	points[11] = msdAdd(poly->offsetsY, context->offsetTrY, size);
	points[12] = msdAdd(poly->heights, context->heights, size);

	nm32s* temp1 = context->buffer1;
	nm32s* temp2 = nmppsAddr_32s(context->buffer1, NMGL_SIZE);

	int height = size / SMALL_SIZE;
	int tail = size % SMALL_SIZE;
	int startTail = size / SMALL_SIZE;
	startTail *= SMALL_SIZE;
	
	nmppmCopy_32s((nm32s*)context->ppPtrns2_2s, 0, temp1, SMALL_SIZE, height, SMALL_SIZE);
	nmppsCopy_32s((nm32s*)context->ppPtrns2_2s, temp1 + startTail, tail);
	CHECK_STATUS(0);
#ifdef __GNUC__
	nmppsAdd_32s((nm32s*)temp1, (nm32s*)polyTmp->ptrnSizesOf32_01, (nm32s*)temp0, size);
#else
	for (int i = 0; i < size; i++) {
		temp0[i] = (int)((int*)temp1[i] + polyTmp->ptrnSizesOf32_01[i]);
	}
#endif
	int dstStride = 3 * SMALL_SIZE;
	nmppmCopy_32s((nm32s*)context->ppPtrns1_2s, 0, (nm32s*)context->ppDstPackPtrns, dstStride, height, SMALL_SIZE);
	nmppmCopy_32s((nm32s*)context->ppPtrns2_2s, 0, (nm32s*)context->ppDstPackPtrns + SMALL_SIZE, dstStride, height, SMALL_SIZE);
	nmppmCopy_32s((nm32s*)temp0, SMALL_SIZE, (nm32s*)context->ppDstPackPtrns + 2 * SMALL_SIZE, dstStride, height, SMALL_SIZE);

	int* dstTail = nmppsAddr_32s((nm32s*)context->ppDstPackPtrns, 3 * startTail);
	int* srcTail12 = nmppsAddr_32s((nm32s*)temp0, startTail);
	nmppsCopy_32s((nm32s*)context->ppPtrns1_2s, dstTail, tail);
	nmppsCopy_32s((nm32s*)context->ppPtrns2_2s, dstTail + tail, tail);
	nmppsCopy_32s((nm32s*)srcTail12, dstTail + 2 * tail, tail);

	CHECK_STATUS(1);
	nmppsAndC_32u((nm32u*)polyTmp->pointInImage, 1, (nm32u*)temp1, size);
	nmppsSub_32s(polyTmp->pointInImage, temp1, polyTmp->pointInImage, size);
#ifdef __GNUC__	
	nmppsAddC_32s(polyTmp->pointInImage, (int)context->colorSegment.data, (nm32s*)context->imagePoints, size);
	nmppsAddC_32s(polyTmp->pointInImage, (int)context->depthSegment.data, (nm32s*)context->zBuffPoints, size);
#else
	for (int i = 0; i < size; i++) { 
		context->imagePoints[i] = (int*)context->colorSegment.data + polyTmp->pointInImage[i];
		context->zBuffPoints[i] = (int*)context->depthSegment.data + polyTmp->pointInImage[i];
	}
#endif
	CHECK_STATUS(2);
	nmppsAdd_32s(polyTmp->widths, temp1, temp2, size);
	nmppsAndC_32u((nm32u*)temp2, 1, (nm32u*)temp0, size);
	nmppsAdd_32s(temp2, temp0, context->widths, size);
	CHECK_STATUS(3);
	nmppsSub_32s(polyTmp->offsetsX, temp1, context->offsetTrX, size);

#ifdef __GNUC__	
	CHECK_STATUS(4);
	nmppsMulC_AddC_32s(polyTmp->numbersPattrns01, WIDTH_PTRN * HEIGHT_PTRN / 16, (int)patterns->ptrns, temp1, size);
	CHECK_STATUS(5);
	nmppsMulC_AddC_32s(polyTmp->numbersPattrns12, WIDTH_PTRN * HEIGHT_PTRN / 16, (int)patterns->ptrns, temp2, size);
	CHECK_STATUS(6);
	nmppsMulC_AddC_32s(polyTmp->numbersPattrns02, WIDTH_PTRN * HEIGHT_PTRN / 16, (int)patterns->ptrns, temp0, size);
#else
	for (int i = 0; i < size; i++) {
		temp1[i] = (int)(&patterns->ptrns[polyTmp->numbersPattrns01[i] * WIDTH_PTRN * HEIGHT_PTRN / 16]);
		temp2[i] = (int)(&patterns->ptrns[polyTmp->numbersPattrns12[i] * WIDTH_PTRN * HEIGHT_PTRN / 16]);
		temp0[i] = (int)(&patterns->ptrns[polyTmp->numbersPattrns02[i] * WIDTH_PTRN * HEIGHT_PTRN / 16]);
	}
#endif
	mergePtrnsAddr3((nm32s**)temp0, (nm32s**)temp1, (nm32s**)temp2, SMALL_SIZE, context->ppSrcPackPtrns, size);

	CHECK_STATUS(7);
	nmppsConvert_32s8s(polyTmp->color, (nm8s*)context->valuesC, 4 * size);

	CHECK_STATUS(8);
	CHECK_STATUS(9);
	mergePtrnsAddr3((nm32s**)polyTmp->ptrnSizesOf32_02, 
		(nm32s**)polyTmp->ptrnSizesOf32_01, 
		(nm32s**)polyTmp->ptrnSizesOf32_12,
		SMALL_SIZE,
		(nm32s**)context->nSizePtrn32,
	 size);
	return 0;
}