#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgldef.h"
#include <nmpp.h>
#include <stdio.h>

SECTION(".text_demo3d") void drawLines(NMGL_Context_NM1* context) {
	PolygonsConnector connector(context->polygonsData);
	Polygons* poly = connector.ptrTail();

	getAddrPtrnsL(context, context->patterns, poly);

	int countTr = poly->count;
	(*connector.pTail)++;

	nm32s* mulBuffer = context->buffer0;
	nm32s* maskBuffer = context->buffer1;
	int point = 0;
	while ( countTr > 0 ) {
		int localSize = MIN(countTr, SMALL_SIZE);
		int point_x2 = point * 2;
		copyPacket_32s(&context->ppSrcPackPtrns[point_x2], 
			&context->ppDstPackPtrns[point_x2], 
			&context->nSizePtrn32[point_x2], 2 * localSize);

		mAndVxN_32u((nm32u**)context->ppPtrns1_2s, 
			(nm32u**)context->ppPtrns2_2s, 
			(nm32u**)context->ppPtrnsCombined_2s_basic, 
			&context->nSizePtrn32[point_x2], localSize);

		nmppsMulC_AddV_AddC_32s(&context->offsetTrY[point], WIDTH_PTRN / 16,
			(nm32s*)context->ppPtrnsCombined_2s_basic, 0,
			(nm32s*)context->ppPtrnsCombined_2s, localSize);

		if (context->depthBuffer->enabled == NMGL_FALSE) {
			mMulCVxN_2s32sExt(context->ppPtrnsCombined_2s,
				&context->offsetTrX[point],
				&context->widths[point],
				&context->heights[point],
				(nm32s*)maskBuffer,
				context->minusOne, localSize);
		}
		else {
			mMulCVxN_2s32sExt(context->ppPtrnsCombined_2s,
				&context->offsetTrX[point],
				&context->widths[point],
				&context->heights[point],
				(nm32s*)mulBuffer,
				&context->valuesZ[point], localSize);

			depthTest(&context->zBuffPoints[point], WIDTH_SEG,
				(nm32s*)mulBuffer,
				(nm32s*)maskBuffer,
				&context->heights[point],
				&context->widths[point], localSize);
		}

		//color v4nm8s in imgOffset
		mMulCVxN_2s_v4nm8sExt(context->ppPtrnsCombined_2s,
			&context->offsetTrX[point],
			&context->widths[point],
			&context->heights[point],
			(v4nm8s*)mulBuffer, (v4nm8s*)&context->valuesC[point], localSize);

		mMaskVxN_32s((nm32s*)mulBuffer,
			(nm32s*)maskBuffer,
			&context->imagePoints[point], WIDTH_SEG,
			&context->heights[point], &context->widths[point], localSize);

		countTr -= SMALL_SIZE;
		point += SMALL_SIZE;
	}

	return;
}
