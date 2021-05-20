#include "demo3d_nm1.h"

extern volatile int copyImageCounterColor;
extern volatile int copyImageCounterDepth;

SECTION(".text_demo3d") void NMGL_CopySegmentFromImage(NMGL_Context_NM1 *context, CommandNm1 *command){
	int x0 = command->params[0].i;
	int y0 = command->params[1].i;
	int width = command->params[2].i;
	int height = command->params[3].i;
	int numOfSeg = command->params[4].i;
	//msdWaitDma(1);
	if (context->depthBuffer.enabled == NMGL_TRUE) {
		context->depthBuffer.setCursor(x0, y0);
		context->smallDepthBuff.setCursor(0, 0);
		while (copyImageCounterDepth <= numOfSeg);
		context->depthBuffer.pushWindow(context->smallDepthBuff, width, height);
	}
	context->colorBuffer.setCursor(x0, y0);
	context->smallColorBuff.setCursor(0, 0);
	while (copyImageCounterColor <= numOfSeg);
	context->colorBuffer.pushWindow(context->smallColorBuff, width, height);
	context->texState.segX0 = x0;
	context->texState.segY0 = y0;
}

SECTION(".text_demo3d") void NMGL_CopySegmentToImage(NMGL_Context_NM1 *context, CommandNm1 *command){
	int x0 = command->params[0].i;
	int y0 = command->params[1].i;
	int width = command->params[2].i;
	int height = command->params[3].i;
	if (context->depthBuffer.enabled == NMGL_TRUE) {
		context->depthBuffer.setCursor(x0, y0);
		context->smallDepthBuff.setCursor(0, 0);
		context->depthBuffer.popWindow(context->smallDepthBuff, width, height);
	}
	context->colorBuffer.setCursor(x0, y0);
	context->smallColorBuff.setCursor(0, 0);
	context->colorBuffer.popWindow(context->smallColorBuff, width, height);
}