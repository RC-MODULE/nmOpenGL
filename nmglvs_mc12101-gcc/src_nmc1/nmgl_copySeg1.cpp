#include "demo3d_nm1.h"
#include "myserverdma.h"

SECTION(".text_demo3d") void NMGL_CopySegmentFromImage(NMGL_Context_NM1 *context, NM_Command *command){
	NMGL_Context *cntxt = NMGL_GetCurrentContext();
	NMGL_Context_NM1 *localContext = NMGL_Context_NM1::getContext();

	int x0 = command->params[0].i;
	int y0 = command->params[1].i;
	int width = command->params[2].i;
	int height = command->params[3].i;
	int numOfSeg = command->params[4].i;

	msdWaitDma(2);
	localContext->frameConnector.readColor(context->innerFramebuffer.buffers[0], &cntxt->defaultFrameBuffer, x0, y0, width, height);

	if (cntxt->depth_test.enabled){
		localContext->frameConnector.readDepth(context->innerFramebuffer.buffers[2], &cntxt->defaultFrameBuffer, x0, y0, width, height);
	}

	msdStart();

	context->texState.segX0 = x0;
	context->texState.segY0 = y0;

	context->segmentSize.width = width;
	context->segmentSize.height = height; 
}

SECTION(".text_demo3d") void NMGL_CopySegmentToImage(NMGL_Context_NM1 *context, NM_Command *command){
	NMGL_Context *cntxt = NMGL_GetCurrentContext();
	NMGL_Context_NM1 *localContext = NMGL_Context_NM1::getContext();

	int x0 = command->params[0].i;
	int y0 = command->params[1].i;
	int width = command->params[2].i;
	int height = command->params[3].i;

	localContext->frameConnector.writeColor(context->innerFramebuffer.buffers[0], &cntxt->defaultFrameBuffer, x0, y0, width, height);

	if (cntxt->depth_test.enabled){
		localContext->frameConnector.writeDepth(context->innerFramebuffer.buffers[2], &cntxt->defaultFrameBuffer, x0, y0, width, height);
	}

	msdStart();
}