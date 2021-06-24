#include "demo3d_nm1.h"



SECTION(".text_demo3d") void NMGL_SwapBuffer(NMGL_Context_NM1 *context, CommandNm1 *command) {
	msdWaitDma();

	context->imageConnector.incHead();
	context->colorBuffer.mData = context->imageConnector.ptrHead();
	while (context->imageConnector.isFull()) {
		halSleep(10);
	}
}

SECTION(".text_demo3d") void NMGL_Exit(NMGL_Context_NM1 *context, CommandNm1 *command){
	
}



