#include "demo3d_nm1.h"
#include "linkmanager.h"


extern NMGL_Framebuffer defaultFramebuffer;

SECTION(".text_demo3d") void NMGL_SwapBuffer(NMGL_Context_NM1 *context, NM_Command *command) {
	msdWaitDma();

	while (defaultFramebuffer.tail == defaultFramebuffer.head - 1);
	void *frontData = defaultFramebuffer.imageBufferFront.data;
	defaultFramebuffer.imageBufferFront.data = defaultFramebuffer.imageBuffer.data;
	defaultFramebuffer.imageBuffer.data = frontData;
	defaultFramebuffer.head++;	
}

SECTION(".text_demo3d") void NMGL_Exit(NMGL_Context_NM1 *context, NM_Command *command){
	
}



