#include "demo3d_nm1.h"
#include "debugprint.h"



SECTION(".text_demo3d") void NMGL_SwapBuffer(NMGL_Context_NM1 *context, NM_Command *command) {
	msdWaitDma();

	DEBUG_PLOG_LEVEL_1("Waiting");
	while(context->defaultFramebuffer->tail < context->defaultFramebuffer->head){

		halSleep(2);
	}
	DEBUG_PLOG_LEVEL_1("OK\n");
	int *temp = (int *)context->defaultFramebuffer->buffers[0];
	context->defaultFramebuffer->buffers[0] = context->defaultFramebuffer->buffers[1];
	context->defaultFramebuffer->buffers[1] = temp;


	context->colorBuffer.mData = context->defaultFramebuffer->buffers[1];

	context->defaultFramebuffer->head++;
}

SECTION(".text_demo3d") void NMGL_Exit(NMGL_Context_NM1 *context, NM_Command *command){
	
}



