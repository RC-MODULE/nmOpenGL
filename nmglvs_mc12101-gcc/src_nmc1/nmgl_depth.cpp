#include "demo3d_nm1.h"


void depthUpdate();

SECTION(".text_demo3d") void NMGL_DepthMask(NMGL_Context_NM1 *context, NM_Command *command) {
	bool maskEnabled = command->params[0].b;
	context->smallFramebuffer.depthBuffer.mask = maskEnabled;
	depthUpdate();
	return;
}

SECTION(".text_demo3d") void NMGL_DepthFunc(NMGL_Context_NM1 *context, NM_Command *command) {
	int depthMode = command->params[0].i;
	context->smallFramebuffer.depthBuffer.mode = depthMode;
	depthUpdate();
	return;
}


SECTION(".text_demo3d") void NMGL_Depth(NMGL_Context_NM1 *context, NM_Command *command){
	context->smallFramebuffer.depthBuffer.enabled = command->params[0].b;
}