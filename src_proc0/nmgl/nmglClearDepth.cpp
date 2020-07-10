#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClearDepthf(NMGLclampf depth) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	float depth_nm1 = ZBUFF_MAX * depth;
	unsigned int depth_int = (unsigned int)depth_nm1;
	if (depth_int > ZBUFF_MAX) {
		depth_int = ZBUFF_MAX;
	}
	cntxt->synchro.writeInstr(1, NMC1_SET_DEPTH, depth_int);
}
