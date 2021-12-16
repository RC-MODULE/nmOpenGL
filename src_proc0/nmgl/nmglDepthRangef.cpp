#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"
#include "nmpp.h"
#include "ringbuffer.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglDepthRangef(NMGLclampf near,	NMGLclampf far) {
	NMGL_Context *cntxt = NMGL_GetCurrentContext();
	cntxt->viewport.viewportMulZ = (far - near) * 0.5 * ZBUFF_MAX;
	cntxt->viewport.viewportAddZ = (far + near) * 0.5 * ZBUFF_MAX;
}
