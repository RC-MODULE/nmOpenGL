#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include <nmpp.h>
#include "myserverdma.h"
#include "dma.h"

int cbUpdate();

//структура данных с информацией о копировани€х
//SECTION(".data_demo3d") MyDmaServer<MSD_SIZE, MSD_NUM_CHANNELS> dmaServer;
SECTION(".data_demo3d") NM_MemCopyManagerDma dmaServer;

SECTION(".text_demo3d") int cbUpdate() {
	halLedOn(5);
	msdCallback callback = dmaServer.currentChannel->ptrTail()->callback;
	if (callback != 0) {
		callback();
	}
	dmaServer.currentChannel->incTail();
	dmaServer.startNextTask();
	halLedOff(5);
	return 0;
}

SECTION(".text_demo3d") void msdInit() {
	halDmaInit();
	halDmaSetCallback(cbUpdate);
	setHeap(11);
	if (!dmaServer.init()) {
		printf("error width msdInit\n");
	}
	for (int i = 0; i < MSD_NUM_CHANNELS; i++) {
		dmaServer.getChannel(i).memcopyPush = (tmemcopy32)nmppsCopy_32s;
	}
}

SECTION(".text_demo3d") unsigned int msdAdd(const void* src, void* dst, int size, int priority) {
	unsigned int id = dmaServer.getChannel(priority).getHead();
	while (dmaServer.getChannel(priority).isFull()) {
		//halSleep(2);
	}
	MyDmaTask* current = dmaServer.getChannel(priority).ptrHead();
	current->src = src;
	current->dst = dst;
	current->size = size;
	current->type = MSD_DMA;
	current->callback = 0;
	//current->t0 = clock();
	dmaServer.getChannel(priority).incHead();
	dmaServer.startJob();
	return id;
}

SECTION(".text_demo3d") unsigned int msdAdd2D(const void* src, void* dst, unsigned size, unsigned width, unsigned srcStride32, unsigned dstStride32, int priority) {
	unsigned int id = dmaServer.getChannel(priority).getHead();
	while (dmaServer.getChannel(priority).isFull()) {
		//halSleep(2);
	}
	MyDmaTask* current = dmaServer.getChannel(priority).ptrHead();
	current->src = src;
	current->dst = dst;
	current->size = size;
	current->type = MSD_DMA_2D;
	current->width = width;
	current->srcStride = srcStride32;
	current->dstStride = dstStride32;
	current->callback = 0;
	//current->t0 = clock();
	dmaServer.getChannel(priority).incHead();
	dmaServer.startJob();
	return id;
}

SECTION(".text_demo3d") unsigned int msdAdd(MyDmaTask &task, int priority) {
	unsigned int id = dmaServer.getChannel(priority).getHead();
	while (dmaServer.getChannel(priority).isFull()) {
		//halSleep(2);
	}
//	task.t0 = clock();
	dmaServer.getChannel(priority).push(&task, 1);
	dmaServer.startJob();
	return id;
}

SECTION(".text_demo3d") bool msdGetStatusCopy(int index, int priority) {
	return dmaServer.getChannel(priority).getTail() > index;
}

SECTION(".text_demo3d") void msdWaitDma() {
	volatile int a = 0;
	while (!dmaServer.isJobCompleted()) {
		//halSleep(2);
		a++;
	}
}

SECTION(".text_demo3d") void msdWaitDma(int channel) {
	volatile int a = 0;
	while (!dmaServer.isChannelCompleted(channel)) {
		//halSleep(2);
		a++;
	}
}


