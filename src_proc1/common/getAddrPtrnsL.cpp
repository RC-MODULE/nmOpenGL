#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"


#define CHECK_STATUS(a) while (dataCopy.statusData[a] == 0)

SECTION(".text_demo3d") int getAddrPtrnsL(NMGL_Context_NM1* context, Patterns* patterns, Polygons* poly){
	Polygons* polyTmp = (Polygons*)context->buffer0;
	int size = poly->count;

	nmppsCopy_32s((int*)poly, (int*)polyTmp, sizeof32(Polygons));

	nm32s* temp0 = nmppsAddr_32s(context->buffer0, sizeof32(Polygons));
	nm32s* temp1 = nmppsAddr_32s(context->buffer1, size);
	nm32s* startPtrnSideAddr = nmppsAddr_32s(temp0, size);
	nm32s* xC = nmppsAddr_32s(temp1, size);
	nm32s* temp2 = nmppsAddr_32s(xC, size);
	nm32s* dydx = nmppsAddr_32s(temp2, size);
	halDmaStartA(patterns->table_dydx, dydx, (2 * WIDTH_PTRN) * (2 * HEIGHT_PTRN + 2));

	nm2s* ptrnsRight_2s = (nm2s*)patterns->ptrns;
	nm2s* ptrnsLeft_2s = nmppsAddr_2s((nm2s*)patterns->ptrns, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN / 2);

	nmppsMulC_32s(polyTmp->dy01, WIDTH_PTRN / 16, (nm32s*)temp0, size);
	nmppsMerge_32s(temp0, temp0, context->nSizePtrn32, size);

	int height = size / SMALL_SIZE;
	int tail = size % SMALL_SIZE;
	int startTail = size / SMALL_SIZE;
	startTail *= SMALL_SIZE;
	int* pointStart = (int*)context->ppDstPackPtrns + 2 * startTail;
	nmppsMerge_32s((nm32s*)context->ppPtrns1_2s, (nm32s*)context->ppPtrns2_2s, temp0, SMALL_SIZE);
	nmppmCopy_32s((nm32s*)temp0, 0, (nm32s*)context->ppDstPackPtrns, 2 * SMALL_SIZE, height, SMALL_SIZE);
	nmppsCopy_32s((nm32s*)temp0, (nm32s*)pointStart, 2 * tail);
	
	nmppsSub_32s(polyTmp->x0, polyTmp->minX, (nm32s*)xC, size);
	while (halDmaIsCompleted());

	for (int i = 0; i < size; i++) {
		if (ABS(polyTmp->dx01[i]) <= polyTmp->dy01[i]) {
			context->ppSrcPackPtrns[2 * i] = (int*)ptrnsRight_2s +
				(dydx[polyTmp->dy01[i] * 2 * WIDTH_PTRN + polyTmp->dx01[i] + WIDTH_PTRN] + xC[i]) *
				WIDTH_PTRN*HEIGHT_PTRN / 16;
			context->ppSrcPackPtrns[2 * i + 1] = (int*)ptrnsLeft_2s +
				(dydx[polyTmp->dy01[i] * 2 * WIDTH_PTRN + polyTmp->dx01[i] + WIDTH_PTRN] + xC[i]) *
				WIDTH_PTRN*HEIGHT_PTRN / 16;
		}
		else {
			if (polyTmp->dx01[i] > 0) {
				context->ppSrcPackPtrns[2 * i] = (int*)ptrnsRight_2s +
					(dydx[polyTmp->dy01[i] * 2 * WIDTH_PTRN + polyTmp->dx01[i] + WIDTH_PTRN] + xC[i]) *
					WIDTH_PTRN*HEIGHT_PTRN / 16;
				context->ppSrcPackPtrns[2 * i + 1] = (int*)ptrnsLeft_2s +
					(dydx[polyTmp->dy01[i] * 2 * WIDTH_PTRN + polyTmp->dx01[i] + WIDTH_PTRN] + xC[i]) *
					WIDTH_PTRN*HEIGHT_PTRN / 16 + WIDTH_PTRN / 16;
			}
			else {
				context->ppSrcPackPtrns[2 * i] = (int*)ptrnsRight_2s +
					(dydx[polyTmp->dy01[i] * 2 * WIDTH_PTRN + polyTmp->dx01[i] + WIDTH_PTRN] + xC[i]) *
					WIDTH_PTRN*HEIGHT_PTRN / 16 + WIDTH_PTRN / 16;
				context->ppSrcPackPtrns[2 * i + 1] = (int*)ptrnsLeft_2s +
					(dydx[polyTmp->dy01[i] * 2 * WIDTH_PTRN + polyTmp->dx01[i] + WIDTH_PTRN] + xC[i]) *
					WIDTH_PTRN*HEIGHT_PTRN / 16;
			}
		}
	}


	//проверка на minY<0
	absIfNegElse0(polyTmp->y0, temp0, size);
	nmppsMulC_32s(temp0, WIDTH_PTRN / 16, context->offsetTrY, size);
	nmppsClipCC_32s(polyTmp->y0, 0, 0x7FFFFFFF, polyTmp->y0, size);
	nmppsSub_32s(polyTmp->dy01, temp0, polyTmp->dy01, size);

	//проверка на minX<0
	absIfNegElse0(polyTmp->minX, temp1, size);
	nmppsClipCC_32s(polyTmp->minX, 0, 0x7FFFFFFF, temp0, size);
	//проверка на невыровненность
	nmppsAndC_32u((nm32u*)temp0, 0xFFFFFFFE, (nm32u*)polyTmp->minX, size);
	nmppsAndC_32u((nm32u*)temp0, 0x1, (nm32u*)temp2, size);
	nmppsSub_32s(temp1, temp2, temp0, size);
	nmppsCopy_32s(temp0, context->offsetTrX, size);
	//temp0 = offsetX
	//temp1 = absIfNegElse0

	//обработка maxDiff
	//Если minX < 0, то maxDiff уменьшается на модуль minX
	//Если maxDiff нечетный, то он округляется к большему четному путем прибавки 1
	//Если offset = -1 (что означает, что треугольник надо выровнить для вставки в изображение),
	//то ширина увеличивается на 2 (чтобы обработать начальный и конечный хвосты)
	nmppsSub_32s(polyTmp->maxDiffX, temp1, polyTmp->maxDiffX, size);
	nmppsAndC_32u((nm32u*)polyTmp->maxDiffX, 0x1, (nm32u*)temp2, size);
	nmppsAdd_32s(polyTmp->maxDiffX, temp2, polyTmp->maxDiffX, size);
	nmppsCmpLt0_32s(temp0, temp2, size);
	nmppsAndC_32u((nm32u*)temp2, 0x2, (nm32u*)temp0, size);
	nmppsAdd_32s(polyTmp->maxDiffX, temp0, polyTmp->maxDiffX, size);

	// проверка на maxX >= segWidth
	nmppsAdd_32s(polyTmp->minX, polyTmp->maxDiffX, temp0, size);		//maxX
	nmppsClipCC_32s(temp0, 0, poly->segWidth, temp1, size);
	nmppsSub_32s(temp1, polyTmp->minX, temp2, size);
	nmppsAddC_32s(temp2, 2, context->widths, size);

	//проверка на maxY >= segHeight
	nmppsAdd_32s(polyTmp->y0, polyTmp->dy01, temp0, size);		//maxY
	nmppsClipCC_32s(temp0, 0, poly->segHeight, temp1, size);
	nmppsAddC_32s(temp1, 1, temp0, size);
	nmppsSub_32s(temp0, polyTmp->y0, context->heights, size);

	nmppsMulC_AddV_AddC_32s(polyTmp->y0,
		WIDTH_SEG,
		polyTmp->minX,
		(int)context->colorSegment.data,
		(nm32s*)context->imagePoints,
		size);
	//nmppsConvert_32s8s(polyTmp->color, (nm8s*)context->valuesC, 4 * size);
	nmppsSet_32s(context->valuesC, 0xFFFFFFFF, size);
	if (context->depthBuffer->enabled) {
		nmppsCopy_32s(polyTmp->z, context->valuesZ, size);
		nmppsMulC_AddV_AddC_32s(polyTmp->y0,
			WIDTH_SEG,
			polyTmp->minX,
			(int)context->depthSegment.data,
			(nm32s*)context->zBuffPoints,
			size);
	}
	return 0;
}