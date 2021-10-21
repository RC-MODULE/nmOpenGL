#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglShadeModel (NMGLenum mode){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if ((mode == NMGL_FLAT) || (mode == NMGL_SMOOTH)) {

		cntxt->shadeModel = mode;	

		NM_Command command;
		command.instr = NMC1_SHADE_MODEL;
		command.params[0] = CommandArgument(mode);
		NMGL_SetValue(command);
	}
	else {
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
