#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"
#include "debugprint.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglDepthFunc (NMGLenum func){
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (func >= NMGL_NEVER && func <= NMGL_ALWAYS) {
		NM_Command command;
		command.instr = NMC1_DEPTH_FUNC;
		command.params[0] = CommandArgument(func);
		NMGL_SetValue(command);
	}
	else {
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
