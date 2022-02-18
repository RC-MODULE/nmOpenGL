#include "demo3d_nm0.h"
#include "nmgl.h"


SECTION(".text_nmgl")
void nmglScissor (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (width < 0 || height < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	cntxt->scissorTest.origin.x = x;
	cntxt->scissorTest.origin.y = y;
	cntxt->scissorTest.size.width = width;
	cntxt->scissorTest.size.height = height;
	int endX = x + width;
	int endY = y + height;
	int nSegments = 0;
	for (int y0 = y; y0 < endY; y0 += HEIGHT_SEG) {
		for (int x0 = x; x0 < endX; x0 += WIDTH_SEG, nSegments++) {
			int localWidth = MIN(WIDTH_SEG, endX - x0);
			int localHeight = MIN(HEIGHT_SEG, endY - y0);
			cntxt->scissorTest.segments.rectangles[nSegments].x = x0;
			cntxt->scissorTest.segments.rectangles[nSegments].y = y0;
			cntxt->scissorTest.segments.rectangles[nSegments].width = localWidth;
			cntxt->scissorTest.segments.rectangles[nSegments].height = localHeight;
			cntxt->scissorTest.segments.lowerLeft[nSegments].v0 = x0;
			cntxt->scissorTest.segments.lowerLeft[nSegments].v1 = y0;
			cntxt->scissorTest.segments.upperRight[nSegments].v0 = x0 + localWidth;
			cntxt->scissorTest.segments.upperRight[nSegments].v1 = y0 + localHeight;
		}
	}
	cntxt->scissorTest.segments.count = nSegments;
}
