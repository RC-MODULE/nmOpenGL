#include "demo3d_nm0.h"
#include "nmpp.h"
#include "stdio.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"
SECTION(".data_imu7") int viewportValues[4];
SECTION(".data_imu7") float viewportValues_f[4];

SECTION(".text_nmgl")
void nmglViewport(NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height) {
	viewportValues[0] = width / 2;
	viewportValues[1] = x + width / 2;
	viewportValues[2] = -height / 2;
	viewportValues[3] = y + height / 2;
	nmppsConvert_32s32f(viewportValues, viewportValues_f, 4);
	cntxt.windowInfo.viewportMulX = viewportValues_f[0];
	cntxt.windowInfo.viewportAddX = viewportValues_f[1];
	cntxt.windowInfo.viewportMulY = viewportValues_f[2];
	cntxt.windowInfo.viewportAddY = viewportValues_f[3];
	int widthSeg = cntxt.windowInfo.segmentWidth;
	int heightSeg = cntxt.windowInfo.segmentHeight;
	cntxt.windowInfo.nColumns = width / widthSeg;
	cntxt.windowInfo.nRows = height / heightSeg;
	for (int i = 0; i < cntxt.windowInfo.nColumns; i++) {
		cntxt.windowInfo.x0[i] = x + i * (widthSeg);
		cntxt.windowInfo.x1[i] = x + widthSeg + i * (widthSeg);
	}
	cntxt.windowInfo.x1[cntxt.windowInfo.nColumns - 1] = width;
	for (int i = 0; i < cntxt.windowInfo.nRows; i++) {
		cntxt.windowInfo.y0[i] = y + i * (heightSeg);
		cntxt.windowInfo.y1[i] = y + heightSeg + i * (heightSeg);
	}
	cntxt.windowInfo.y1[cntxt.windowInfo.nRows - 1] = height;

	nmppsConvert_32s32f(cntxt.windowInfo.x0, cntxt.windowInfo.x0_f, cntxt.windowInfo.nColumns);
	nmppsConvert_32s32f(cntxt.windowInfo.x1, cntxt.windowInfo.x1_f, cntxt.windowInfo.nColumns);
	nmppsConvert_32s32f(cntxt.windowInfo.y0, cntxt.windowInfo.y0_f, cntxt.windowInfo.nRows);
	nmppsConvert_32s32f(cntxt.windowInfo.y1, cntxt.windowInfo.y1_f, cntxt.windowInfo.nRows);
}