#include "demo3d_nm1.h"



SECTION(".text_demo3d") void NMGL_SetColor(NMGL_Context_NM1 *context, CommandNm1 *command) {
	int red = command->params[0];
	int green = command->params[1];
	int blue = command->params[2];
	int alpha = command->params[3];
	context->colorBuffer.clearColor(red, green, blue, alpha);
	context->smallColorBuff.mClearValue = context->colorBuffer.mClearValue;
	context->smallClearColorBuff.mClearValue = context->colorBuffer.mClearValue;
	return;
}

SECTION(".text_demo3d") void NMGL_SetDepth(NMGL_Context_NM1 *context, CommandNm1 *command) {
	context->depthBuffer.mClearValue = command->params[0];
	context->smallDepthBuff.mClearValue = context->depthBuffer.mClearValue;
	context->smallClearDepthBuff.mClearValue = context->depthBuffer.mClearValue;
	return;
}

SECTION(".text_demo3d") void NMGL_PointSize(NMGL_Context_NM1 *context, CommandNm1 *command) {
	context->pointSize = command->params[0];
	return;
}


