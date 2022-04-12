#include "demo3d_nm0.h"
#include "nmpp.h"
#include "stdio.h"
#include "nmgl.h"
#include "segment.h"
#include "debugprint.h"


#pragma code_section ".text_nmgl"
SECTION(".data_imu7") int viewportValues[4];
SECTION(".data_imu7") float viewportValues_f[4];


SECTION(".text_nmgl")
void nmglViewport(NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height) {
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0* cntxt = NMGL_Context_NM0::getContext();
	if (width < 0 || height < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}	
	cntxt->windowInfo.viewportMulX = width / 2;
	cntxt->windowInfo.viewportAddX = x + width / 2;
	cntxt->windowInfo.viewportMulY = height / 2;
	cntxt->windowInfo.viewportAddY = y + height / 2;
	segmentsInit(&cntxt->windowInfo.segments, {x, y}, {width, height}, {WIDTH_SEG,HEIGHT_SEG});
}