#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglHint (NMGLenum target, NMGLenum mode)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	if ((target != NMGL_PERSPECTIVE_CORRECTION_HINT	&&
		 target != NMGL_LINE_SMOOTH_HINT 			&&
		 target != NMGL_POINT_SMOOTH_HINT)          ||	
		(mode   != NMGL_NICEST						&&
		 mode   != NMGL_FASTEST						&&
		 mode   != NMGL_DONT_CARE)
		) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	switch (target){
	case NMGL_PERSPECTIVE_CORRECTION_HINT: 
										cntxt->perspectiveCorrectionHint = mode;
										break;
	case NMGL_LINE_SMOOTH_HINT:
										cntxt->lineSmoothHint = mode;
										break;
	case NMGL_POINT_SMOOTH_HINT:
										cntxt->pointSmoothHint = mode;
										break;
	}
	
	NM_Command command;
	command.instr = NMC1_SET_HINT;
	command.params[0] = CommandArgument(target);
	command.params[1] = CommandArgument(mode);
	NMGL_SetValue(command);
}
