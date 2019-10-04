#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>


SECTION(".text_demo3d") void drawTriangles(NMGL_Context_NM1* context) {
	Polygons* poly = (Polygons*)halRingBufferTail(context->polygonsRB);

	getAddrPtrnsT(context, context->patterns, poly);

	msdWaitDma();
	int countTrangles = poly->count;
	context->polygonsRB->tail++;

	nm32s* mulBuffer = context->buffer0;
	nm32s* maskBuffer = context->buffer1;
	

	int point = 0;
	while (countTrangles > 0) {
		int localSize = MIN(countTrangles, SMALL_SIZE);
		int point_x3 = point * 3;
		int* widths = &context->widths[point];
		int* heights = &context->heights[point];
		int* offsetsX = &context->offsetTrX[point];
		int* valuesC = &context->valuesC[point];
		int* valuesZ = &context->valuesZ[point];


		copyPacket_32s(&context->ppSrcPackPtrns[point_x3], 
			&context->ppDstPackPtrns[point_x3], 
			&context->nSizePtrn32[point_x3], 3 * localSize);

		mAndVxN_32u((nm32u**)context->ppPtrns1_2s, 
			(nm32u**)context->ppPtrns2_2s, 
			(nm32u**)context->ppPtrnsCombined_2s_basic, 
			&context->nSizePtrn32[point_x3], localSize);

		nmppsMulC_AddV_AddC_32s(&context->offsetTrY[point], WIDTH_PTRN / 16, 
			(nm32s*)context->ppPtrnsCombined_2s_basic, 0,
			(nm32s*)context->ppPtrnsCombined_2s, localSize);

		if (context->depthBuffer->enabled == NMGL_FALSE) {
			mMulCVxN_2s32sExt(context->ppPtrnsCombined_2s,
				offsetsX,
				widths,
				heights,
				(nm32s*)maskBuffer,
				context->minusOne, localSize);
		}
		else {
			mMulCVxN_2s32sExt(context->ppPtrnsCombined_2s,
				offsetsX,
				widths,
				heights,
				(nm32s*)mulBuffer,
				valuesZ, localSize);

			depthTest(&context->zBuffPoints[point], WIDTH_SEG,
				(nm32s*)mulBuffer,
				(nm32s*)maskBuffer,
				heights,
				widths, localSize);
		}

		//color v4nm8s in imgOffset
		mMulCVxN_2s_v4nm8sExt(context->ppPtrnsCombined_2s,
			offsetsX,
			widths,
			heights,
			(v4nm8s*)mulBuffer, (v4nm8s*)valuesC, localSize);

		mMaskVxN_32s((nm32s*)mulBuffer,
			(nm32s*)maskBuffer,
			&context->imagePoints[point], WIDTH_SEG,
			heights, widths, localSize);

		countTrangles -= SMALL_SIZE;
		point += SMALL_SIZE;
	}
	printf("\n");
	return;
}
