#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include <nmpp.h>
#include "myserverdma.h"


//структура данных с информацией о копировани€х
SECTION(".data_demo3d") HalRingBufferData<MSD_DmaCopy, 128> msdRingBufferCopy;
SECTION(".data_demo3d") static volatile bool msdDmaIsBusy = false;
SECTION(".data_demo3d") static volatile bool msdIsCriticalSection = false;
SECTION(".data_demo3d") static volatile bool msdInUpdate = false;
SECTION(".data_demo3d") static volatile int msdSemafor = 0;;

#ifdef __GNUC__
#define msdSingleCopy(src, dst, size32)  halDmaStart(src, dst, size32);
#define msdMatrixCopy(src, dst, size, width, srcStride, dstStride) \
		halDma2D_Start(src, dst, size, width, srcStride, dstStride);
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

inline int semaforInc(){
	if(msdSemafor){
		printf("inc, semafor=%d\n", msdSemafor + 1);
	}
	return ++msdSemafor;
	
}

inline int semaforDec(){
	//printf("dec, semafor=%d\n", msdSemafor - 1);
	return --msdSemafor;
}

SECTION(".text_demo3d") int cbUpdate() {
	msdRingBufferCopy.tail++;	
	if(semaforInc() == 2){
		return 0;
	}
	if (!msdRingBufferCopy.isEmpty()) {
		msdStartCopy(msdRingBufferCopy.ptrTail());
	}
	semaforDec();
	return 0;
}

SECTION(".text_demo3d") void msdInit() {
	msdRingBufferCopy.init();
#ifdef __GNUC__
	halDmaInitC();
#endif // __GNUC__
	halDmaSetCallback(cbUpdate);
}

SECTION(".text_demo3d") unsigned int msdAdd(const void* src, void* dst, int size) {
	while (msdRingBufferCopy.isFull());
	MSD_DmaCopy* current = msdRingBufferCopy.ptrHead();
	current->src = src;
	current->dst = dst;
	current->size = size;
	current->status = false;
	current->type = MSD_DMA;
	
	semaforInc();
	msdRingBufferCopy.head++;
	semaforDec();
	
	if(msdSemafor){
		semaforDec();
		msdStartCopy(msdRingBufferCopy.ptrTail());
	} else if(msdRingBufferCopy.head - msdRingBufferCopy.tail == 1){
		msdStartCopy(msdRingBufferCopy.ptrTail());
	}
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


