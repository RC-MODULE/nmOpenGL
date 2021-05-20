#include "demo3d_nm1.h"

SECTION(".data_imu0") volatile int copyImageCounterColor;
SECTION(".data_imu0") volatile int copyImageCounterDepth;

SECTION(".text_demo3d") int copyCounterColor() {
	return copyImageCounterColor++;
}

SECTION(".text_demo3d") int copyCounterDepth() {
	return copyImageCounterDepth++;
}


SECTION(".text_demo3d") void NMGL_Clear(NMGL_Context_NM1 *context, CommandNm1 *command){
	msdWaitDma();
	context->smallClearColorBuff.setCursor(0, 0);
	context->smallClearDepthBuff.setCursor(0, 0);
	int widthImage = context->colorBuffer.getWidth();
	int heightImage = context->colorBuffer.getHeight();
	int widthSeg = context->smallClearColorBuff.getWidth();
	int heightSeg = context->smallClearColorBuff.getHeight();
	copyImageCounterColor = 0;
	copyImageCounterDepth = 0;
	switch (command->params[0].i)
	{
	case NMGL_COLOR_BUFFER_BIT:
		context->smallClearColorBuff.clear();
		for (int y = 0; y < heightImage; y += heightSeg) {
			for (int x = 0; x < widthImage; x += widthSeg) {
				context->colorBuffer.setCursor(x, y);
				int width = MIN(widthSeg, widthImage - x);
				int height = MIN(heightSeg, heightImage - y);
				context->smallClearColorBuff.pushWindow(context->colorBuffer, width, height, copyCounterColor);
			}
		}
		break;
	case NMGL_DEPTH_BUFFER_BIT:
		context->smallClearDepthBuff.clear();
		for (int y = 0; y < heightImage; y += heightSeg) {
			for (int x = 0; x < widthImage; x += widthSeg) {
				context->depthBuffer.setCursor(x, y);
				int width = MIN(widthSeg, widthImage - x);
				int height = MIN(heightSeg, heightImage - y);
				context->smallClearDepthBuff.pushWindow(context->depthBuffer, width, height, copyCounterDepth);
			}
		}
		break;
	case NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT:
		context->smallClearColorBuff.clear();
		context->smallClearDepthBuff.clear();
		for (int y = 0; y < heightImage; y += heightSeg) {
			for (int x = 0; x < widthImage; x += widthSeg) {
				context->colorBuffer.setCursor(x, y);
				context->depthBuffer.setCursor(x, y);
				int width = MIN(widthSeg, widthImage - x);
				int height = MIN(heightSeg, heightImage - y);
				context->smallClearColorBuff.pushWindow(context->colorBuffer, width, height, copyCounterColor);
				context->smallClearDepthBuff.pushWindow(context->depthBuffer, width, height, copyCounterDepth);
			}
		}
		break;
	default:
		break;
	}
}