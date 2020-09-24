#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglFlush (){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	v2nm32f *minXY = (v2nm32f*)cntxt->buffer4;
	v2nm32f *maxXY = (v2nm32f*)cntxt->buffer4 + 3 * NMGL_SIZE;
	if (cntxt->lineInner.size) {
		findMinMax2(cntxt->lineInner.x0, cntxt->lineInner.x1,
			cntxt->buffer0, cntxt->buffer1,
			cntxt->lineInner.size);
		findMinMax2(cntxt->lineInner.y0, cntxt->lineInner.y1,
			cntxt->buffer2, cntxt->buffer3,
			cntxt->lineInner.size);
		nmppsMerge_32f(cntxt->buffer0, cntxt->buffer2, (float*)minXY, cntxt->lineInner.size);
		nmppsMerge_32f(cntxt->buffer1, cntxt->buffer3, (float*)maxXY, cntxt->lineInner.size);
		//PROFILER_SIZE(cntxt->trianInner.size);
		setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->lineInner.size);
		//PROFILER_SIZE(cntxt->trianInner.size);
		rasterizeL(&cntxt->lineInner, cntxt->segmentMasks);
		cntxt->lineInner.size = 0;
	}
	if (cntxt->trianInner.size) {
		if (cntxt->isCullFace) {
			cullFaceSortTriangles(cntxt->trianInner);
			if (cntxt->trianInner.size == 0) {
				return;
			}
		}
		findMinMax3(cntxt->trianInner.x0, cntxt->trianInner.x1, cntxt->trianInner.x2,
			cntxt->buffer0, cntxt->buffer1,
			cntxt->trianInner.size);
		findMinMax3(cntxt->trianInner.y0, cntxt->trianInner.y1, cntxt->trianInner.y2,
			cntxt->buffer2, cntxt->buffer3,
			cntxt->trianInner.size);
		nmppsMerge_32f(cntxt->buffer0, cntxt->buffer2, (float*)minXY, cntxt->trianInner.size);
		nmppsMerge_32f(cntxt->buffer1, cntxt->buffer3, (float*)maxXY, cntxt->trianInner.size);
		//PROFILER_SIZE(cntxt->trianInner.size);
		setSegmentMask(minXY, maxXY, cntxt->segmentMasks, cntxt->trianInner.size);
		//PROFILER_SIZE(cntxt->trianInner.size);
		rasterizeT(&cntxt->trianInner, cntxt->segmentMasks);
		cntxt->trianInner.size = 0;
	}
	
}
