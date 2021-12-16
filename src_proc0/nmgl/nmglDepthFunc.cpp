#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglDepthFunc (NMGLenum func){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (func >= NMGL_NEVER && func <= NMGL_ALWAYS) {
		context->depth_test.func = func;
		NM_Command command;
		command.instr = NMC1_DEPTH_UPDATE;
		command.params[0] = CommandArgument(func);
		NMGL_SetValue(command);
	}
	else {
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
