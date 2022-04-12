#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"
#include "debugprint.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglDepthMask (NMGLboolean flag){
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NM_Command command;
	if(flag != NMGL_FALSE) flag = NMGL_TRUE;
	cntxt->depth_write_mask_enabled = flag;
	command.instr = NMC1_DEPTH_MASK;
	command.params[0] = CommandArgument(flag);
	NMGL_SetValue(command);
}
