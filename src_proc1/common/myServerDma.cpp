#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include <nmpp.h>
#include "myserverdma.h"

int cbUpdate();

//структура данных с информацией о копировани€х
SECTION(".data_demo3d") MyDmaServer<MSD_SIZE, MSD_NUM_CHANNELS> dmaServer;
SECTION(".data_demo3d") MyDmaClient<MSD_SIZE> dmaClient[MSD_NUM_CHANNELS];

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
	dmaServer.init(cbUpdate);
	for (int i = 0; i < MSD_NUM_CHANNELS; i++) {
		dmaClient[i].bind(dmaServer.getChannelBuffer(i));
		dmaClient[i].memcopyPush = (tmemcopy32)nmppsCopy_32s;
	}
}

SECTION(".text_demo3d") unsigned int msdAdd(const void* src, void* dst, int size, int priority) {
	unsigned int id = dmaClient[priority].getHead();
	while (dmaClient[priority].isFull()) {
		//halSleep(2);
	}
	MyDmaTask* current = dmaClient[priority].ptrHead();
	current->src = src;
	current->dst = dst;
	current->size = size;
	current->type = MSD_DMA;
	current->callback = 0;
	//current->t0 = clock();
	dmaClient[priority].incHead();
	dmaServer.startJob();
	return id;
}

SECTION(".text_demo3d") unsigned int msdAdd2D(const void* src, void* dst, unsigned size, unsigned width, unsigned srcStride32, unsigned dstStride32, int priority) {
	unsigned int id = dmaClient[priority].getHead();
	while (dmaClient[priority].isFull()) {
		//halSleep(2);
	}
	MyDmaTask* current = dmaClient[priority].ptrHead();
	current->src = src;
	current->dst = dst;
	current->size = size;
	current->type = MSD_DMA_2D;
	current->width = width;
	current->srcStride = srcStride32;
	current->dstStride = dstStride32;
	current->callback = 0;
	//current->t0 = clock();
	dmaClient[priority].incHead();
	dmaServer.startJob();
	return id;
}

SECTION(".text_demo3d") unsigned int msdAdd(MyDmaTask &task, int priority) {
	unsigned int id = dmaClient[priority].getHead();
	while (dmaClient[priority].isFull()) {
		//halSleep(2);
	}
//	task.t0 = clock();
	dmaClient[priority].add(task);
	dmaServer.startJob();
	return id;
}

SECTION(".text_demo3d") bool msdGetStatusCopy(int index, int priority) {
	return dmaClient[priority].getTail() > index;
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


