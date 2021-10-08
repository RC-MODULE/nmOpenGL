#include "demo3d_nm1.h"

SECTION(".text_demo3d") void NMGL_ShadeModel(NMGL_Context_NM1 *context, NM_Command *command) {
	context->shadeModel=command->params[0].i;
	return;
}

