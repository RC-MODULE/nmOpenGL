#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglDepthFunc (NMGLenum func){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (func >= NMGL_NEVER && func <= NMGL_ALWAYS) {
		CommandNm1 command;
		command.instr = NMC1_DEPTH_FUNC;
		command.params[0] = CommandArgument(func);
		cntxt->synchro.pushInstr(&command);
	}
	else {
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
