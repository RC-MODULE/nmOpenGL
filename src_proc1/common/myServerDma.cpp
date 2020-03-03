#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include <nmpp.h>
#include "myserverdma.h"

int cbUpdate();

//структура данных с информацией о копировани€х
SECTION(".data_demo3d") MyDmaServer<MSD_SIZE, MSD_NUM_CHANNELS> dmaServer(cbUpdate);
SECTION(".data_demo3d") MyDmaClient<MSD_SIZE> dmaClient[MSD_NUM_CHANNELS];
SECTION(".data_demo3d") clock_t t0Wait, t1Wait;
SECTION(".data_demo3d") int waitingTime[MSD_NUM_CHANNELS];


SECTION(".text_demo3d") int cbUpdate() {
	MyDmaTask* currentTask = dmaServer.currentChannel->ptrTail();
	msdCallback callback = currentTask->callback;
	if (callback != 0) {
		callback();
	}
	currentTask->t1 = clock();
	dmaServer.time[currentTask->priority] += currentTask->t1 - currentTask->t0;
	dmaServer.size[currentTask->priority] += currentTask->size;
	dmaServer.currentChannel->incTail();
	dmaServer.startNextTask();
	return 0;
}

SECTION(".text_demo3d") void msdInit() {
	for (int i = 0; i < MSD_NUM_CHANNELS; i++) {
		dmaClient[i].bind(dmaServer.getChannelBuffer(i));
		dmaClient[i].memcopyPush = (tmemcopy32)nmppsCopy_32s;
	}
}

SECTION(".text_demo3d") unsigned int msdAdd(const void* src, void* dst, int size, int priority) {
	unsigned int id = dmaClient[priority].getHead();
	while (dmaClient[priority].isFull());
	MyDmaTask* current = dmaClient[priority].ptrHead();
	current->src = src;
	current->dst = dst;
	current->size = size;
	current->type = MSD_DMA;
	current->callback = 0;
	current->priority = priority;
	dmaClient[priority].incHead();
	dmaServer.startJob();
	return id;
}

SECTION(".text_demo3d") unsigned int msdAdd2D(const void* src, void* dst, unsigned size, unsigned width, unsigned srcStride32, unsigned dstStride32, int priority) {
	unsigned int id = dmaClient[priority].getHead();
	while (dmaClient[priority].isFull());
	MyDmaTask* current = dmaClient[priority].ptrHead();
	current->src = src;
	current->dst = dst;
	current->size = size;
	current->type = MSD_DMA_2D;
	current->width = width;
	current->srcStride = srcStride32;
	current->dstStride = dstStride32;
	current->callback = 0;
	current->priority = priority;
	dmaClient[priority].incHead();
	dmaServer.startJob();
	return id;
}

SECTION(".text_demo3d") unsigned int msdAdd(MyDmaTask &task, int priority) {
	unsigned int id = dmaClient[priority].getHead();
	while (dmaClient[priority].isFull());
	task.priority = priority;
	dmaClient[priority].add(task);
	dmaServer.startJob();
	return id;
}

SECTION(".text_demo3d") bool msdGetStatusCopy(int index, int priority) {
	return dmaClient[priority].getTail() > index;
}

SECTION(".text_demo3d") void msdWaitIndexCopy(int index, int priority) {
	volatile int a = 0;
	t0Wait = clock();
	while (!msdGetStatusCopy(index, priority)) {
		a++;
	}
	t1Wait = clock();
	waitingTime[priority] += t1Wait - t0Wait;
}

SECTION(".text_demo3d") void msdWaitDma() {
	volatile int a = 0;
	t0Wait = clock();
	while (!dmaServer.isJobCompleted()) {
		a++;
	}
	t1Wait = clock();
	int endWait = t1Wait - t0Wait;
	
	for (int i = 0; i < MSD_NUM_CHANNELS; i++) {
		printf("time dma[%d]=%d, size dma=%d\n", i, dmaServer.time[i], dmaServer.size[i]);
		dmaServer.time[i] = 0;
		dmaServer.size[i] = 0;
		
	}
	for (int i = 0; i < MSD_NUM_CHANNELS; i++) {
		printf("wait time dma[%d]=%d\n", i, waitingTime[i]);
		waitingTime[i] = 0;
	}
	printf("end wait =%d\n", endWait);
	
}

SECTION(".text_demo3d") void msdWaitDma(int channel) {
	volatile int a = 0;
	t0Wait = clock();
	while (!dmaServer.isChannelCompleted(channel)) {
		a++;
	}
	t1Wait = clock();
	waitingTime[channel] += t1Wait - t0Wait;
}


