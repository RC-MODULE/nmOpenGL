#include "demo3d_nm1.h"
#include "myserverdma.h"

extern NMGL_Framebuffer defaultFramebuffer;

SECTION(".text_demo3d") void NMGL_CopySegmentFromImage(NMGL_Context_NM1 *context, NM_Command *command){
	NMGL_Framebuffer *bigFrame = &defaultFramebuffer;
	NMGL_Framebuffer *smallFrame = &context->smallFramebuffer;
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
	task.srcStride = bigFrame->getWidth();
	task.dstStride = smallFrame->getWidth();
	if (smallFrame->depthBuffer.enabled == NMGL_TRUE) {
		task.src = (int*)bigFrame->depthBuffer.data + y0 * bigFrame->getWidth() + x0;
		task.dst = smallFrame->depthBuffer.data;
		msdAdd(task, 1);
	}
	task.src = (int*)bigFrame->imageBuffer.data + y0 * bigFrame->getWidth() + x0;
	task.dst = smallFrame->imageBuffer.data;
	msdAdd(task, 1);

	context->texState.segX0 = x0;
	context->texState.segY0 = y0;

	context->segmentSize.width = width;
	context->segmentSize.height = height;
}

SECTION(".text_demo3d") void NMGL_CopySegmentToImage(NMGL_Context_NM1 *context, NM_Command *command){
	NMGL_Framebuffer *bigFrame = &defaultFramebuffer;
	NMGL_Framebuffer *smallFrame = &context->smallFramebuffer;
	int x0 = command->params[0].i;
	int y0 = command->params[1].i;
	int width = command->params[2].i;
	int height = command->params[3].i;
	MyDmaTask task;
	task.type = MSD_DMA_2D;
	task.size = width * height;
	task.width = width;
	task.srcStride = smallFrame->getWidth();
	task.dstStride = bigFrame->getWidth();
	if (smallFrame->depthBuffer.enabled == NMGL_TRUE) {
		task.src = smallFrame->depthBuffer.data;
		task.dst = (int*)bigFrame->depthBuffer.data + y0 * bigFrame->getWidth() + x0;		
		msdAdd(task, 1);
	}
	task.src = smallFrame->imageBuffer.data;
	task.dst = (int*)bigFrame->imageBuffer.data + y0 * bigFrame->getWidth() + x0;
	msdAdd(task, 1);
}