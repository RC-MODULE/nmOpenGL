#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglDepthMask (NMGLboolean flag){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CommandNm1 command;
	command.instr = NMC1_DEPTH_MASK;
	command.params[0] = CommandArgument(flag);
	cntxt->synchro.pushInstr(&command);
}
