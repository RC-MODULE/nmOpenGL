#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"
#include "nmpp.h"
#include "ringbuffer.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglDepthRangef(NMGLclampf near,	NMGLclampf far) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->windowInfo.viewportMulZ = (far - near) * 0.5 * ZBUFF_MAX;
	cntxt->windowInfo.viewportAddZ = (far + near) * 0.5 * ZBUFF_MAX;
}
