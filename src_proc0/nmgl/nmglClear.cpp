#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClear(NMGLbitfield mask) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	NM_Command command;
	command.instr = NMC1_CLEAR;
	command.params[0] = CommandArgument(mask);
	if (context->scissorTest.isEnabled) {
		command.params[1] = CommandArgument(context->scissorTest.origin.x);
		command.params[2] = CommandArgument(context->scissorTest.origin.y);
		command.params[3] = CommandArgument(context->scissorTest.size.width);
		command.params[4] = CommandArgument(context->scissorTest.size.height);
	}
	else {
		command.params[1] = CommandArgument(0);
		command.params[2] = CommandArgument(0);
		command.params[3] = CommandArgument(context->defaultFrameBuffer.width);
		command.params[4] = CommandArgument(context->defaultFrameBuffer.height);
	}
	
	NMGL_SetValue(command);
}
