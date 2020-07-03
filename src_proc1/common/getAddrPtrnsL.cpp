#include "nmpp.h"
#include "dma.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include "stdio.h"
#include "service.h"
#include "imagebuffer.h"

extern unsigned int points[14];

extern MyDmaTask task;

#define CHECK_STATUS(a) while (!msdGetStatusCopy(points[a], 0))

inline void ADD_COPY(const void* src, void* dst, int size, int i) {
	task.src = src;
	task.dst = dst;
	task.size = size;
	points[i] = msdAdd(task, 0);
}

SECTION(".text_demo3d") int getAddrPtrnsL(NMGL_Context_NM1* context, Polygons* poly) {
	PatternsArray* patterns = context->patterns;
	int size = poly->count;

	for (int i = 0; i < size; i++) {
		if (poly->pointInImage[i] & 1) {
			poly->pointInImage[i]--;
			poly->widths[i]++;
			poly->offsetsX[i]--;
		}
		if (poly->widths[i] & 1) {
			poly->widths[i]++;
		}
		context->ptrnInnPoints[i].x = poly->offsetsX[i];
		context->ptrnInnPoints[i].y = poly->offsetsY[i];
		context->ptrnSizes[i].width = poly->widths[i];
		context->ptrnSizes[i].height = poly->heights[i];

		context->valuesZ[i] = poly->z[i];

		context->ppSrcPackPtrns[2 * i] = context->patterns->
			ptrns[poly->numbersPattrns01[i] ];
		context->ppSrcPackPtrns[2 * i + 1] = context->patterns->
			ptrns[poly->numbersPattrns01[i] + NPATTERNS / 2];

		context->ppDstPackPtrns[2 * i] = (int*)context->ppPtrns1_2s[i % SMALL_SIZE];
		context->ppDstPackPtrns[2 * i + 1] = (int*)context->ppPtrns2_2s[i % SMALL_SIZE];

		context->nSizePtrn32[2 * i] = poly->ptrnSizesOf32_01[i];
		context->nSizePtrn32[2 * i + 1] = poly->ptrnSizesOf32_01[i];

		context->imagePoints[i] = (int*)context->smallColorBuff.mData + poly->pointInImage[i];
		context->zBuffPoints[i] = (int*)context->smallDepthBuff.mData + poly->pointInImage[i];

	}

#ifdef OUTPUT_IMAGE_RGB8888
	nmppsConvert_32s8s(poly->color, (nm8s*)context->valuesC, 4 * size);
	//inverse_v4nm8u((v4nm8u*)temp1, (v4nm8u*)context->valuesC, size);
#endif // OUTPUT_IMAGE_RGB8888

#ifdef OUTPUT_IMAGE_RGB565
	convertABGR32_RGB565((abgr32*)polyTmp->color, (rgb565*)temp0, size);
	nmppsConvert_16u32u((nm16u*)temp0, (nm32u*)context->valuesC, size);
#endif // OUTPUT_IMAGE_RGB565

	return 0;
}