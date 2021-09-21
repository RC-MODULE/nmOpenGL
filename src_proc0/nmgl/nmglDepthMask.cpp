#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglDepthMask (NMGLboolean flag){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NM_Command command;
	command.instr = NMC1_DEPTH_MASK;
	command.params[0] = CommandArgument(flag);
	NMGL_SetValue(command);
}
