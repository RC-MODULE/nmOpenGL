#include "demo3d_nm1.h"
#include "linkmanager.h"


extern NMGL_Framebuffer defaultFramebuffer;

bool linkIsInited = false;

SECTION(".data_demo3d") NM_MemCopyManagerLink<0, LINK_OUTPUT> linkManager;

SECTION(".text_demo3d") void lmCallback() {
	LinkCallback callback = linkManager.currentChannel->ptrTail()->callback;
	if (callback != 0) {
		callback();
	}
	linkManager.currentChannel->incTail();
	linkManager.startNextTask();
}

SECTION(".text_demo3d") void lmInit() {
	halLinkInit(0, LINK_OUTPUT);
	halLinkSetCallback(lmCallback, 0, LINK_OUTPUT);
	int heapNo = getHeap();
	setHeap(11);
	if (!linkManager.init()) {
		printf("linkIn error\n");
	}
	setHeap(heapNo);
}


SECTION(".text_demo3d") void lmAddTransfer(LinkTask &task) {
	linkManager.add(&task);
	linkManager.startJob();
}

SECTION(".text_demo3d") void lmWaitTransfer() {
	while(linkManager.isJobCompleted() == 0);
}

SECTION(".data_demo3d") NM_Command segInfo;

SECTION(".text_demo3d") void NMGL_SwapBuffer(NMGL_Context_NM1 *context, NM_Command *command) {
	msdWaitDma();

	while (defaultFramebuffer.tail == defaultFramebuffer.head - 1);
	void *frontData = defaultFramebuffer.imageBufferFront.data;
	defaultFramebuffer.imageBufferFront.data = defaultFramebuffer.imageBuffer.data;
	defaultFramebuffer.imageBuffer.data = frontData;
	defaultFramebuffer.head++;


	if(!linkIsInited){
		lmInit();
		linkIsInited = true;
	}

	NMGL_Framebuffer *smallBuffer = &context->smallFramebuffer;
	LinkTask task;

	for(int i = 0; i < defaultFramebuffer.getSize(); i+=smallBuffer->getSize()){
		int localSize = MIN(defaultFramebuffer.getSize() - i, smallBuffer->getSize());
		halCopyRISC((int*)defaultFramebuffer.imageBufferFront.data + i, smallBuffer->imageBuffer.data, localSize);
		segInfo.instr = NMC1_COPY_SEG_TO_IMAGE;
		segInfo.params[0] = CommandArgument(i);
		segInfo.params[1] = CommandArgument(localSize);
		task.data = &segInfo;
		task.size32 = sizeof32(NM_Command);
		lmAddTransfer(task);
		lmWaitTransfer();
		task.data = smallBuffer->imageBuffer.data;
		task.size32 = localSize;
		lmAddTransfer(task);
		lmWaitTransfer();
	}
	lmWaitTransfer();
	segInfo.instr = NMC1_SWAP_BUFFER;
	task.data = &segInfo;
	task.size32 = sizeof32(NM_Command);
	lmAddTransfer(task);
	lmWaitTransfer();
	defaultFramebuffer.tail++;
	
}

SECTION(".text_demo3d") void NMGL_Exit(NMGL_Context_NM1 *context, NM_Command *command){
	
}



