#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"
#include "context.h"

#include "imagebuffer.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClearColor(NMGLclampf red, NMGLclampf green, NMGLclampf blue, NMGLclampf alpha) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	context->clear.color[0] = red;
	context->clear.color[1] = green;
	context->clear.color[2] = blue;
	context->clear.color[3] = alpha;

	NM_Command command;
	command.instr = NMC1_SET_COLOR;
	command.params[0] = CommandArgument((int)(red * RED_COEFF));
	command.params[1] = CommandArgument((int)(green * GREEN_COEFF));
	command.params[2] = CommandArgument((int)(blue * BLUE_COEFF));
	command.params[3] = CommandArgument((int)(alpha * ALPHA_COEFF));
	//NMGL_SetValue(command);
}

