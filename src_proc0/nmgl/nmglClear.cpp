#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClear(NMGLbitfield mask) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CommandNm1 command;
	command.instr = NMC1_CLEAR;
	command.params[0] = CommandArgument(mask);
	if (cntxt->scissorTest.isEnabled) {
		command.params[1] = CommandArgument(cntxt->scissorTest.origin.x);
		command.params[2] = CommandArgument(cntxt->scissorTest.origin.y);
		command.params[3] = CommandArgument(cntxt->scissorTest.size.width);
		command.params[4] = CommandArgument(cntxt->scissorTest.size.height);
	}
	else {
		command.params[1] = CommandArgument(0);
		command.params[2] = CommandArgument(0);
		command.params[3] = CommandArgument(cntxt->windowInfo.imageSize.width);
		command.params[4] = CommandArgument(cntxt->windowInfo.imageSize.height);
	}
	
	cntxt->synchro.pushInstr(&command);
}
