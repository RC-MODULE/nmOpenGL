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
	for (int i = 0;i < 8;i++) {
		context->colorClearValueTwice[i] = mClearValue;
	}
	//context->smallClearColorBuff.mClearValue = context->colorBuffer.mClearValue;
	return;
}

SECTION(".text_demo3d") void NMGL_SetDepth(NMGL_Context_NM1 *context, NM_Command *command) {
	long long unsigned mClearValue = command->params[0].ui;
	mClearValue |= (mClearValue << 32);
	for (int i = 0;i < 8;i++) {
		context->depthClearValueTwice[i] = mClearValue;
	}
	//context->smallClearDepthBuff.mClearValue = context->depthBuffer.mClearValue;
	return;
}



