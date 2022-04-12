#include "demo3d_nm0.h"
#include "nmgl.h"
#include "segment.h"
#include "debugprint.h"


SECTION(".text_nmgl")
void nmglScissor (NMGLint x, NMGLint y, NMGLsizei width, NMGLsizei height){
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (width < 0 || height < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	cntxt->scissorTest.origin.x = x;
	cntxt->scissorTest.origin.y = y;
	cntxt->scissorTest.size.width = width;
	cntxt->scissorTest.size.height = height;
	segmentsInit(&cntxt->scissorTest.segments, cntxt->scissorTest.origin, cntxt->scissorTest.size, {WIDTH_SEG,HEIGHT_SEG});
}
