#include "demo3d_nm1.h"



SECTION(".text_demo3d") void NMGL_SetColor(NMGL_Context_NM1 *context, NM_Command *command) {
	unsigned int red = command->params[0].ui;
	unsigned int green = command->params[1].ui;
	unsigned int blue = command->params[2].ui;
	unsigned int alpha = command->params[3].ui;
	long long unsigned mClearValue = blue;
	mClearValue |= (green << 8);
	mClearValue |= (red << 16);
	mClearValue |= (alpha << 24);
	mClearValue |= (mClearValue << 32);
	context->colorBuffer.clearColor(red, green, blue, alpha);
	context->smallColorBuff.mClearValue = context->colorBuffer.mClearValue;
	for (int i = 0;i < 8;i++) {
		context->colorClearValueTwice[i] = mClearValue;
	}
	//context->smallClearColorBuff.mClearValue = context->colorBuffer.mClearValue;
	return;
}

SECTION(".text_demo3d") void NMGL_SetDepth(NMGL_Context_NM1 *context, NM_Command *command) {
	context->depthBuffer.mClearValue = command->params[0].ui;
	context->smallDepthBuff.mClearValue = context->depthBuffer.mClearValue;
	long long unsigned mClearValue = command->params[0].ui;
	mClearValue |= (mClearValue << 32);
	for (int i = 0;i < 8;i++) {
		context->depthClearValueTwice[i] = mClearValue;
	}
	//context->smallClearDepthBuff.mClearValue = context->depthBuffer.mClearValue;
	return;
}

SECTION(".text_demo3d") void NMGL_PointSize(NMGL_Context_NM1 *context, NM_Command *command) {
	context->pointSize = command->params[0].i;
	return;
}



SECTION(".text_demo3d") void NMGL_AlphaTestSet(NMGL_Context_NM1 *context, NM_Command *command){
	context->alpha_test.enabled = command->params[0].b;
}

SECTION(".text_demo3d") void NMGL_BlendSet(NMGL_Context_NM1 *context, NM_Command *command){
	context->blend.enabled = command->params[0].b;
}


SECTION(".text_demo3d") void NMGL_PolygonOffsetFill(NMGL_Context_NM1 *context, NM_Command *command){
	context->polygon.offset_fill_enabled = command->params[0].b;
}

SECTION(".text_demo3d") void NMGL_SetHint(NMGL_Context_NM1 *context, NM_Command *command){

	NMGLenum target = command->params[0].ui;
	NMGLenum mode = command->params[1].ui;

	switch (target){
	case NMGL_PERSPECTIVE_CORRECTION_HINT: 
										context->perspectiveCorrectionHint = mode;
										break;
	case NMGL_LINE_SMOOTH_HINT:
										context->lineSmoothHint = mode;
										break;
	case NMGL_POINT_SMOOTH_HINT:
										context->pointSmoothHint = mode;
										break;
	}
}


