#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"

SECTION(".data_demo3d") unsigned int points[14];

SECTION(".data_demo3d") MyDmaTask task;

#define CHECK_STATUS(a) while (!msdGetStatusCopy(points[a], 0))

inline void ADD_COPY(const void* src, void* dst, int size, int i) {
	task.src = src;
	task.dst = dst;
	task.size = size;
	points[i] = msdAdd(task, 0);
}

SECTION(".text_demo3d") int getAddrPtrnsT(NMGL_Context_NM1* context, Polygons* poly){
	Polygons* polyTmp = (Polygons*)context->buffer0;
	PatternsArray* patterns = context->patterns;
	int size = poly->count;
	nm32s* temp0 = nmppsAddr_32s(context->buffer0, sizeof32(Polygons));
	nm32s* dydx = nmppsAddr_32s(temp0, NMGL_SIZE);
	task.type = MSD_DMA;
	task.callback = 0;
	ADD_COPY(poly->ptrnSizesOf32_01, polyTmp->ptrnSizesOf32_01, size, 0);
	ADD_COPY(poly->pointInImage, polyTmp->pointInImage, size, 1);
	ADD_COPY(poly->widths, polyTmp->widths, size, 2);
	ADD_COPY(poly->offsetsX, polyTmp->offsetsX, size, 3);
	ADD_COPY(poly->offsetsY, polyTmp->offsetsY, size, 4);
	ADD_COPY(poly->heights, polyTmp->heights, size, 5);
	ADD_COPY(poly->numbersPattrns01, polyTmp->numbersPattrns01, size, 6);
	ADD_COPY(poly->numbersPattrns12, polyTmp->numbersPattrns12, size, 7);
	ADD_COPY(poly->numbersPattrns02, polyTmp->numbersPattrns02, size, 8);
	ADD_COPY(poly->color, polyTmp->color, 4 * size, 9);
	ADD_COPY(poly->ptrnSizesOf32_02, polyTmp->ptrnSizesOf32_02, size, 10);
	ADD_COPY(poly->z, context->valuesZ, size, 11);

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
	//nmppsRShiftC_32u((nm32u*)polyTmp->pointInImage, 1, (nm32u*)polyTmp->pointInImage, size);
	nmppsAndC_32u((nm32u*)polyTmp->pointInImage, 1, (nm32u*)temp1, size);
	nmppsSub_32s(polyTmp->pointInImage, temp1, polyTmp->pointInImage, size);
#ifdef __GNUC__	
	nmppsAddC_32s(polyTmp->pointInImage, (int)context->smallColorBuff.data, (nm32s*)context->imagePoints, size);
	nmppsAddC_32s(polyTmp->pointInImage, (int)context->smallDepthBuff.data, (nm32s*)context->zBuffPoints, size);
#else
	for (int i = 0; i < size; i++) { 
		context->imagePoints[i] = (int*)context->smallColorBuff.data + polyTmp->pointInImage[i];
		context->zBuffPoints[i] = (int*)context->smallDepthBuff.data + polyTmp->pointInImage[i];
	}
#endif
	CHECK_STATUS(2);
	nmppsAdd_32s(polyTmp->widths, temp1, temp2, size);
	nmppsAddC_32s(temp2, 1, temp0, size);
	nmppsAndC_32u((nm32u*)temp0, 0xFFFFFFFE, (nm32u*)temp2, size);
	CHECK_STATUS(5);
	nmppsSub_32s(polyTmp->offsetsX, temp1, temp0, size);
	nmppsMerge_32s(temp0, polyTmp->offsetsY, (nm32s*)context->ptrnInnPoints, size);
	nmppsMerge_32s(temp2, polyTmp->heights, (nm32s*)context->ptrnSizes, size);	

#ifdef __GNUC__	
	CHECK_STATUS(6);
	nmppsMulC_AddC_32s(polyTmp->numbersPattrns01, sizeof32(Pattern), (int)patterns->ptrns, temp1, size);
	CHECK_STATUS(7);
	nmppsMulC_AddC_32s(polyTmp->numbersPattrns12, sizeof32(Pattern), (int)patterns->ptrns, temp2, size);
	CHECK_STATUS(8);
	nmppsMulC_AddC_32s(polyTmp->numbersPattrns02, sizeof32(Pattern), (int)patterns->ptrns, temp0, size);
#else
	for (int i = 0; i < size; i++) {
		temp1[i] = (int)(&patterns->ptrns[polyTmp->numbersPattrns01[i]]);
		temp2[i] = (int)(&patterns->ptrns[polyTmp->numbersPattrns12[i]]);
		temp0[i] = (int)(&patterns->ptrns[polyTmp->numbersPattrns02[i]]);
	}
#endif
	mergePtrnsAddr3((nm32s**)temp0, (nm32s**)temp1, (nm32s**)temp2, SMALL_SIZE, context->ppSrcPackPtrns, size);

	CHECK_STATUS(9);
	nmppsConvert_32s8s(polyTmp->color, (nm8s*)context->valuesC, 4 * size);
	//convertABGR32_RGB565((abgr32*)polyTmp->color, (rgb565*)temp0, size);
	//duplicate_16s((nm16s*)temp0, context->valuesC, size);
	//convertRGB565_RGB8888((rgb565*)temp0, (rgb8888*)context->valuesC, size);

	CHECK_STATUS(10);
	nmppsSub_32s(polyTmp->ptrnSizesOf32_02, polyTmp->ptrnSizesOf32_01, temp0, size);
	mergePtrnsAddr3((nm32s**)polyTmp->ptrnSizesOf32_02, 
		(nm32s**)polyTmp->ptrnSizesOf32_01, 
		(nm32s**)temp0,
		SMALL_SIZE,
		(nm32s**)context->nSizePtrn32,
	 size);
	//CHECK_STATUS(11);
	//nmppsConvert_32s16s(polyTmp->z, (nm16s*)context->valuesZ, size);
	//nmppsCopy_32s(polyTmp->z, context->valuesZ, size);
	return 0;
}