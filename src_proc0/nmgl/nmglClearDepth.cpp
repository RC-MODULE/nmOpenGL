#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClearDepthf(NMGLclampf depth) {
	float depth_nm1 = ZBUFF_MAX * depth;
	cntxt.synchro->writeInstr(1, NMC1_SET_DEPTH, depth_nm1);
}
