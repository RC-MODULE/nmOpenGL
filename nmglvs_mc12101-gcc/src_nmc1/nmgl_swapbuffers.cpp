#include "demo3d_nm1.h"



SECTION(".text_demo3d") void NMGL_SwapBuffer(NMGL_Context_NM1 *context, NM_Command *command) {
	msdWaitDma();

	NMGL_Context *cntxt = NMGL_GetCurrentContext();

	
	// context->imageConnector.incHead();
	while (cntxt->defaultFrameBuffer.tail < cntxt->defaultFrameBuffer.head){
		halSleep(2);
	}

	int *temp = (int *)cntxt->defaultFrameBuffer.buffers[0];
	cntxt->defaultFrameBuffer.buffers[0] = cntxt->defaultFrameBuffer.buffers[1];
	cntxt->defaultFrameBuffer.buffers[1] = temp;

	cntxt->defaultFrameBuffer.head++;
	
}

SECTION(".text_demo3d") void NMGL_Exit(NMGL_Context_NM1 *context, NM_Command *command){
	
}



