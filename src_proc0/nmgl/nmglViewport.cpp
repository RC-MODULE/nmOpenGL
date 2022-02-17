#include "demo3d_nm0.h"
#include "nmpp.h"
#include "stdio.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"
SECTION(".data_imu7") int viewportValues[4];
SECTION(".data_imu7") float viewportValues_f[4];


SECTION(".text_nmgl")
void nmglViewport(NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height) {
	NMGL_Context_NM0* cntxt = NMGL_Context_NM0::getContext();
	if (width < 0 || height < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}	
	cntxt->windowInfo.viewportMulX = width / 2;
	cntxt->windowInfo.viewportAddX = x + width / 2;
	cntxt->windowInfo.viewportMulY = height / 2;
	cntxt->windowInfo.viewportAddY = y - height / 2 + HEIGHT_IMAGE;
	int endX = x + width;
	int endY = y + height;
	int nSegments = 0;
	for (int y0 = y; y0 < endY; y0 += HEIGHT_SEG) {
		for (int x0 = x; x0 < endX; x0 += WIDTH_SEG, nSegments++) {
			int localWidth = MIN(WIDTH_SEG, endX - x0);
			int localHeight = MIN(HEIGHT_SEG, endY - y0);
			cntxt->windowInfo.segments.rectangles[nSegments].x = x0;
			cntxt->windowInfo.segments.rectangles[nSegments].y = HEIGHT_IMAGE - y0 - localHeight;
			cntxt->windowInfo.segments.rectangles[nSegments].width = localWidth;
			cntxt->windowInfo.segments.rectangles[nSegments].height = localHeight;
			cntxt->windowInfo.segments.lowerLeft[nSegments].v0 = x0;
			cntxt->windowInfo.segments.lowerLeft[nSegments].v1 = HEIGHT_IMAGE - y0 - localHeight;
			cntxt->windowInfo.segments.upperRight[nSegments].v0 = x0 + localWidth;
			cntxt->windowInfo.segments.upperRight[nSegments].v1 = HEIGHT_IMAGE - y0;
		}
	}

	
	cntxt->windowInfo.segments.count = nSegments;
}