#include "demo3d_nm1.h"
#include "myserverdma.h"
#include "nmprofiler.h"

SECTION(".data_demo3d")	MyDmaTask clearTasks[2]; ///clearTasks: 0 - color, 1-depth

SECTION(".dama_demo3d") volatile unsigned int bufferClearCounters[2];
SECTION(".dama_demo3d") volatile unsigned int bufferClearStep[2];

#define STEP 16384

SECTION(".text_demo3d") int colorBufferClearPartCallback() {
	bufferClearCounters[0] += bufferClearStep[0];
	return bufferClearCounters[0];
}

SECTION(".text_demo3d") int depthBufferClearPartCallback() {
	bufferClearCounters[1] += bufferClearStep[1];
	return bufferClearCounters[1];
}


SECTION(".text_demo3d") void NMGL_Clear(NMGL_Context_NM1 *context, NM_Command *command){
	
	int widthImage = context->colorBuffer.getWidth();
	int heightImage = context->colorBuffer.getHeight();
	//

	msdWaitDma();
	clearTasks[0].src = context->colorClearValueTwice;
	clearTasks[0].width = 16;
	clearTasks[0].srcStride = 0;
	clearTasks[0].dstStride = 16;
	clearTasks[0].type = MSD_DMA_2D;
	clearTasks[0].callback = colorBufferClearPartCallback;

	clearTasks[1].src = context->depthClearValueTwice;	
	clearTasks[1].width = 16;	
	clearTasks[1].srcStride = 0;	
	clearTasks[1].dstStride = 16;	
	clearTasks[1].type = MSD_DMA_2D;		
	clearTasks[1].callback = depthBufferClearPartCallback;


	int x0 = command->params[1].i;
	int y0 = command->params[2].i;
	int width = command->params[3].i;
	int height = command->params[4].i;

	//printf("address=%p, value=%xl\n", context->colorClearValueTwice, context->colorClearValueTwice[0]);

	switch (command->params[0].i)
	{
	case NMGL_COLOR_BUFFER_BIT: {
		if (x0 <= 0 && (width + x0) >= widthImage) {
			bufferClearCounters[0] = 0;
			bufferClearStep[0] = STEP;
			int size = width * height;
			int origin = x0 + y0 * width;
			for (int i = 0; i < size; i += STEP) {
				int localSize = MIN(STEP, size - i);
				clearTasks[0].dst = context->colorBuffer.setCursor(i + origin);
				clearTasks[0].size = localSize;
				msdAdd(clearTasks[0], 2);
			}
		}
		else {
			bufferClearCounters[0] = x0 + y0 * widthImage;
			bufferClearStep[0] = widthImage;
			for (int y = 0; y < height; y++) {
				clearTasks[0].dst = context->colorBuffer.setCursor(x0, y + y0);
				clearTasks[0].size = width;
				msdAdd(clearTasks[0], 2);
			}
		}
	}
	break;
	case NMGL_DEPTH_BUFFER_BIT: {
		if (x0 <= 0 && (width + x0) >= widthImage) {
			bufferClearCounters[1] = 0;
			bufferClearStep[1] = STEP;
			int size = width * height;
			int origin = x0 + y0 * width;
			for (int i = 0; i < size; i += STEP) {
				int localSize = MIN(STEP, size - i);
				clearTasks[1].dst = context->depthBuffer.setCursor(i + origin);
				clearTasks[1].size = localSize;
				msdAdd(clearTasks[1], 2);
			}
		}
		else {
			bufferClearCounters[1] = x0 + y0 * widthImage;
			bufferClearStep[1] = widthImage;
			for (int y = 0; y < height; y++) {
				clearTasks[1].dst = context->depthBuffer.setCursor(x0, y + y0);
				clearTasks[1].size = width;
				msdAdd(clearTasks[1], 2);
			}
		}
	}
	break;
	case NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT: {
		if (x0 <=0 && (width + x0) >= widthImage) {
			bufferClearCounters[0] = 0;
			bufferClearCounters[1] = 0;
			bufferClearStep[0] = STEP;
			bufferClearStep[1] = STEP;
			int size = width * height;
			int origin = x0 + y0 * width;
			for (int i = 0; i < size; i += STEP) {
				int localSize = MIN(STEP, size - i);
				clearTasks[0].dst = context->colorBuffer.setCursor(i + origin);
				clearTasks[0].size = localSize;
				clearTasks[1].dst = context->depthBuffer.setCursor(i + origin);
				clearTasks[1].size = localSize;
				msdAdd(clearTasks[0], 2);
				msdAdd(clearTasks[1], 2);
			}
		}
		else {
			bufferClearCounters[0] = x0 + y0 * widthImage;
			bufferClearCounters[1] = x0 + y0 * widthImage;
			bufferClearStep[0] = widthImage;
			bufferClearStep[1] = widthImage;
			for (int y = 0; y < height; y++) {
				clearTasks[0].dst = context->colorBuffer.setCursor(x0, y + y0);
				clearTasks[0].size = width;
				clearTasks[1].dst = context->depthBuffer.setCursor(x0, y + y0);
				clearTasks[1].size = width;
				msdAdd(clearTasks[0], 2);
				msdAdd(clearTasks[1], 2);
			}
		}
	}
	break;
	default:
		break;
	}
	
}