#include "demo3d_nm0.h"
#include "nmgl.h"
#include "math.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglPointSize(NMGLfloat size) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (size <= 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	cntxt->pointRadius = floor(size / 2);
	CommandNm1 command;
	command.instr = NMC1_POINT_SIZE;
	command.params[0] = CommandArgument(size);
	//cntxt->synchro.writeInstr(1, NMC1_POINT_SIZE, (int)size);
	cntxt->synchro.pushInstr(&command);
}
