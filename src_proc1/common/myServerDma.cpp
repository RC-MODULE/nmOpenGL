#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include <nmpp.h>
#include "myserverdma.h"


//структура данных с информацией о копированиях
SECTION(".data_demo3d") HalRingBufferData<MSD_DmaCopy, 128> msdRingBufferCopy;
SECTION(".data_demo3d") volatile bool msdDmaIsBusy = false;
SECTION(".data_demo3d") volatile bool msdFlagPause = false;

#ifdef __GNUC__
#define msdSingleCopy(src, dst, size32)  halDmaStartCA(src, dst, size32);
#define msdMatrixCopy(src, dst, size, width, srcStride, dstStride) \
		halDma2D_StartCA(src, dst, size, width, srcStride, dstStride);
#else
int empty() { return 0; }

DmaCallback cb = empty;
#define halDmaSetCallback(func) cb = func;

#define msdSingleCopy(src, dst, size) halCopyRISC(src,dst,size); cb()
#define msdMatrixCopy(src, dst, size, width, srcStride, dstStride) \
			 nmppmCopy_32s((nm32s*)src, srcStride, \
							(nm32s*)dst, dstStride, \
							size/width, width); cb()
#endif

inline void msdStartCopy(MSD_DmaCopy* dmaCopy) {
	switch (dmaCopy->type)
	{
	case MSD_DMA:
		msdSingleCopy(dmaCopy->src, dmaCopy->dst, dmaCopy->size);
		break;
	case MSD_DMA_2D:
		msdMatrixCopy(dmaCopy->src, dmaCopy->dst,
			dmaCopy->size, dmaCopy->width,
			dmaCopy->srcStride, dmaCopy->dstStride);
		break;
	default:
		break;
	}
}

SECTION(".text_demo3d") void cbUpdate() {
	msdRingBufferCopy.tail++;
	if (!msdFlagPause && !msdRingBufferCopy.isEmpty()) {
		msdStartCopy(msdRingBufferCopy.ptrTail());
	}
	else {
		msdDmaIsBusy = false;
	}
}

SECTION(".text_demo3d") void msdInit() {
	msdRingBufferCopy.init();
	msdDmaIsBusy = false;
	msdFlagPause = false;
#ifdef __GNUC__
	halDmaInitC();
#endif // __GNUC__
	halDmaSetCallback((DmaCallback)cbUpdate);
}

SECTION(".text_demo3d") unsigned int msdAdd(const void* src, void* dst, int size) {
	while (msdRingBufferCopy.isFull());
	MSD_DmaCopy* current = msdRingBufferCopy.ptrHead();
	current->src = src;
	current->dst = dst;
	current->size = size;
	current->status = false;
	current->type = MSD_DMA;

	msdFlagPause = true;
	while (msdDmaIsBusy);
	msdFlagPause = false;
	msdDmaIsBusy = true; 
	msdRingBufferCopy.head++;
	msdStartCopy(msdRingBufferCopy.ptrTail());
	return msdRingBufferCopy.head - 1;
}

SECTION(".text_demo3d") unsigned int msdAdd2D(const void* src, void* dst, unsigned size, unsigned width, unsigned srcStride32, unsigned dstStride32) {
	while (msdRingBufferCopy.isFull());
	MSD_DmaCopy* current = msdRingBufferCopy.ptrHead();
	current->src = src;
	current->dst = dst;
	current->size = size;
	current->status = false;
	current->type = MSD_DMA_2D;
	current->width = width;
	current->srcStride = srcStride32;
	current->dstStride = dstStride32;

	msdFlagPause = true;	//вход в критическую секцию
	while (msdDmaIsBusy);
	msdFlagPause = false;
	msdDmaIsBusy = true;
	msdRingBufferCopy.head++;
	msdStartCopy(msdRingBufferCopy.ptrTail());
	return msdRingBufferCopy.head - 1;
}

SECTION(".text_demo3d") bool msdGetStatusCopy(int index) {
	return msdRingBufferCopy.tail > index;
}

SECTION(".text_demo3d") void msdWaitDma() {
	volatile int a = 0;
	while (!msdRingBufferCopy.isEmpty()) {
		a++;
	}
}


