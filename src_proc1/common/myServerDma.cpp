#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include <nmpp.h>
#include "myserverdma.h"


//структура данных с информацией о копированиях
SECTION(".data_demo3d") HalRingBufferData<MSD_DmaCopy, 128> msdRingBufferCopy;
SECTION(".data_demo3d") volatile bool msdDmaIsBusy = false;
SECTION(".data_demo3d") volatile bool msdIsCriticalSection = false;

#ifdef __GNUC__
#define msdSingleCopy(src, dst, size32)  halDmaStartA(src, dst, size32);
#define msdMatrixCopy(src, dst, size, width, srcStride, dstStride) \
		halDma2D_StartA(src, dst, size, width, srcStride, dstStride);
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



SECTION(".text_demo3d") void cbUpdate() {
	msdRingBufferCopy.tail++;
	//если update вызывается в критической секции, то конвеер копирований прерывается
	if (msdIsCriticalSection && msdDmaIsBusy) {
		msdDmaIsBusy = false;
		return;
	}
	if (!msdRingBufferCopy.isEmpty()) {
		MSD_DmaCopy* current = msdRingBufferCopy.ptrTail();
		switch (current->type)
		{
		case MSD_DMA:
			msdSingleCopy(current->src, current->dst, current->size);
			break;
		case MSD_DMA_2D:
			msdMatrixCopy(current->src, current->dst,
				current->size, current->width,
				current->srcStride, current->dstStride);
			break;
		default:
			break;
		}
	}
	else {
		msdDmaIsBusy = false;
	}
}

SECTION(".text_demo3d") void msdInit() {
#ifdef __GNUC__
	halDmaInit();
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
	//вход в критическую секцию
	msdIsCriticalSection = true;
	while (msdDmaIsBusy);
	msdRingBufferCopy.head++;
	//если в конвеере больше нет копирований, то запускается первое копирование
	if (msdRingBufferCopy.head - msdRingBufferCopy.tail == 1) {
		msdDmaIsBusy = true;
		msdSingleCopy(src, dst, size);
	}
	//выход из критической секции
	msdIsCriticalSection = false;
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
	//вход в критическую секцию
	msdIsCriticalSection = true;
	while (msdDmaIsBusy);
	msdRingBufferCopy.head++;	
	if (msdRingBufferCopy.head - msdRingBufferCopy.tail == 1) {
		msdDmaIsBusy = true;
		msdMatrixCopy(src, dst, size, width, srcStride32, dstStride32);
	}
	//выход из критической секции
	msdIsCriticalSection = false;
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


