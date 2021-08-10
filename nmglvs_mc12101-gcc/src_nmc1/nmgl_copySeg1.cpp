#include "demo3d_nm1.h"
#include "myserverdma.h"


SECTION(".text_demo3d") void NMGL_CopySegmentFromImage(NMGL_Context_NM1 *context, NM_Command *command){
	int x0 = command->params[0].i;
	int y0 = command->params[1].i;
	int width = command->params[2].i;
	int height = command->params[3].i;
	int numOfSeg = command->params[4].i;
	msdWaitDma(2);
	MyDmaTask task;
	task.type = MSD_DMA_2D;
	task.size = width * height;
	task.width = width;
	if (context->depthBuffer.enabled == NMGL_TRUE) {
		task.src = context->depthBuffer.setCursor(x0, y0); 
		task.dst = context->smallDepthBuff.setCursor(0, 0);
		task.srcStride = context->depthBuffer.getWidth();
		task.dstStride = context->smallDepthBuff.getWidth();
		msdAdd(task, 1);
	}
	task.src = context->colorBuffer.setCursor(x0, y0); 
	task.dst = context->smallColorBuff.setCursor(0, 0);
	task.srcStride = context->colorBuffer.getWidth();
	task.dstStride = context->smallColorBuff.getWidth();
	msdAdd(task, 1);

	context->texState.segX0 = x0;
	context->texState.segY0 = y0;

	context->segmentSize.width = width;
	context->segmentSize.height = height;
}

SECTION(".text_demo3d") void NMGL_CopySegmentToImage(NMGL_Context_NM1 *context, NM_Command *command){
	int x0 = command->params[0].i;
	int y0 = command->params[1].i;
	int width = command->params[2].i;
	int height = command->params[3].i;
	MyDmaTask task;
	task.type = MSD_DMA_2D;
	task.size = width * height;
	task.width = width;
	if (context->depthBuffer.enabled == NMGL_TRUE) {
		task.src = context->smallDepthBuff.setCursor(0, 0);
		task.dst = context->depthBuffer.setCursor(x0, y0);
		task.srcStride = context->smallDepthBuff.getWidth();
		task.dstStride = context->depthBuffer.getWidth();
		msdAdd(task, 1);
	}
	task.src = context->smallColorBuff.setCursor(0, 0);
	task.dst = context->colorBuffer.setCursor(x0, y0);
	task.srcStride = context->smallColorBuff.getWidth();
	task.dstStride = context->colorBuffer.getWidth();
	msdAdd(task, 1);
}