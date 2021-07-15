#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClearDepthf(NMGLclampf depth) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	float depth_nm1 = ZBUFF_MAX * depth;
	unsigned int depth_int = (unsigned int)depth_nm1;
	if (depth_int > ZBUFF_MAX) {
		depth_int = ZBUFF_MAX;
	}
	CommandNm1 command;
	command.instr = NMC1_SET_DEPTH;
	command.params[0] = CommandArgument(depth_int);
	cntxt->synchro.pushInstr(&command);
}
