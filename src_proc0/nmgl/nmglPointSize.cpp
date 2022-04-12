#include "demo3d_nm0.h"
#include "nmgl.h"
#include "math.h"
#include "nmglservice_nm0.h"
#include "debugprint.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglPointSize(NMGLfloat size) {
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (size <= 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	cntxt->pointRadius = size / 2;
	NM_Command command;
	command.instr = NMC1_POINT_SIZE;
	command.params[0] = CommandArgument((int)size);
	NMGL_SetValue(command);
}
