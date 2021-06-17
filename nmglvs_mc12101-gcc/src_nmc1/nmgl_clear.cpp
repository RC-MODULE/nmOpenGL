#include "demo3d_nm1.h"
#include "myserverdma.h"
#include "nmprofiler.h"

SECTION(".data_demo3d")	MyDmaTask clearTasks[2]; ///clearTasks: 0 - color, 1-depth

SECTION(".dama_demo3d") volatile unsigned int bufferClearCounters[2];

#define STEP 16384

SECTION(".text_demo3d") int colorBufferClearPartCallback() {
	bufferClearCounters[0] += STEP;
	return bufferClearCounters[1];
}

SECTION(".text_demo3d") int depthBufferClearPartCallback() {
	bufferClearCounters[1] += STEP;
	return bufferClearCounters[1];
}


extern MyDmaServer<MSD_SIZE, MSD_NUM_CHANNELS> dmaServer;
extern MyDmaClient<MSD_SIZE> dmaClient[MSD_NUM_CHANNELS];


SECTION(".text_demo3d") void NMGL_Clear(NMGL_Context_NM1 *context, CommandNm1 *command){
	
	int widthImage = context->colorBuffer.getWidth();
	int heightImage = context->colorBuffer.getHeight();
	int size = context->colorBuffer.getSize();

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

	//printf("address=%p, value=%xl\n", context->colorClearValueTwice, context->colorClearValueTwice[0]);

	switch (command->params[0].i)
	{
	case NMGL_COLOR_BUFFER_BIT:
		bufferClearCounters[0] = 0;
		for (int i = 0; i < size; i += STEP) {
			int localSize = MIN(STEP, size - i);
			clearTasks[0].size = localSize;
			clearTasks[0].dst = (int*)context->colorBuffer.mData + i;
			msdAdd(clearTasks[0], 2);
		}
		break;
	case NMGL_DEPTH_BUFFER_BIT:
		bufferClearCounters[1] = 0;
		for (int i = 0; i < size; i += STEP) {
			int localSize = MIN(STEP, size - i);
			clearTasks[1].size = localSize;
			clearTasks[1].dst = (int*)context->depthBuffer.mData + i;
			msdAdd(clearTasks[1], 2);
		}
		break;
	case NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT:
		bufferClearCounters[0] = 0;
		bufferClearCounters[1] = 0;
		for (int i = 0; i < size; i += STEP) {
			
			int localSize = MIN(STEP, size - i);
			clearTasks[0].dst = context->colorBuffer.setCursor(i);
			clearTasks[0].size = localSize;
			clearTasks[1].dst = context->depthBuffer.setCursor(i);
			clearTasks[1].size = localSize;
			msdAdd(clearTasks[0], 2);
			msdAdd(clearTasks[1], 2);
		}
		break;
	default:
		break;
	}
	
}