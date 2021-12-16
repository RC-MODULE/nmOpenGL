#include "demo3d_nm0.h"
#include "nmgl.h"


SECTION(".text_nmgl")
void nmglScissor (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (width < 0 || height < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	context->scissorTest.origin.x = x;
	context->scissorTest.origin.y = y;
	context->scissorTest.size.width = width;
	context->scissorTest.size.height = height;
	int endX = x + width;
	int endY = y + height;
	int nSegments = 0;
	for (int y0 = y; y0 < endY; y0 += HEIGHT_SEG) {
		for (int x0 = x; x0 < endX; x0 += WIDTH_SEG, nSegments++) {
			int localWidth = MIN(WIDTH_SEG, endX - x0);
			int localHeight = MIN(HEIGHT_SEG, endY - y0);
			cntxt->scissorSegments.rectangles[nSegments].x = x0;
			cntxt->scissorSegments.rectangles[nSegments].y = HEIGHT_IMAGE - y0 - localHeight;
			cntxt->scissorSegments.rectangles[nSegments].width = localWidth;
			cntxt->scissorSegments.rectangles[nSegments].height = localHeight;
			cntxt->scissorSegments.lowerLeft[nSegments].v0 = x0;
			cntxt->scissorSegments.lowerLeft[nSegments].v1 = HEIGHT_IMAGE - y0 - localHeight;
			cntxt->scissorSegments.upperRight[nSegments].v0 = x0 + localWidth;
			cntxt->scissorSegments.upperRight[nSegments].v1 = HEIGHT_IMAGE - y0;
		}
	}
	cntxt->scissorSegments.count = nSegments;
}
