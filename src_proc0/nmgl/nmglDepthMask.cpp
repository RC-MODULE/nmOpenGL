#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglDepthMask (NMGLboolean flag){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	NM_Command command;
	if(flag != NMGL_FALSE) flag = NMGL_TRUE;
	context->depth_test.mask = flag;
	command.instr = NMC1_DEPTH_UPDATE;
	command.params[0] = CommandArgument(flag);
	NMGL_SetValue(command);
}
