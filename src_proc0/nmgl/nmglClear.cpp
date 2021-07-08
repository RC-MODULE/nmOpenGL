#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClear(NMGLbitfield mask) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CommandNm1 command;
	command.instr = NMC1_CLEAR;
	command.params[0] = CommandArgument(mask);
	cntxt->synchro.pushInstr(&command);
}
