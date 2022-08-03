#ifndef __MY_SERVER_DMA_H__
#define __MY_SERVER_DMA_H__ 
//#include "time.h"
#include "dma.h"
#include "ringbuffert.h"
#include "nmcopymanager.h"

#define MSD_DMA 0
#define MSD_DMA_2D 1
#define MSD_SIZE 256
#define MSD_NUM_CHANNELS 3

typedef int(*msdCallback)();

/*!
 *  \brief Структура для работы с DMA
 *  
 */
struct MyDmaTask {
	const void* src;
	void* dst;
	int size;
	int width;
	int srcStride;
	int dstStride;
	int type;
	msdCallback callback = 0;
};

class NM_MemCopyManagerDma : public NM_MemCopyManager<MyDmaTask, MSD_SIZE, MSD_NUM_CHANNELS, NM_MemCopyManagerDma> {
public:
#ifdef __GNUC__
	void startFunc(MyDmaTask* dmaCopy) {
		if (dmaCopy->type == MSD_DMA) {
			halDmaStartA(dmaCopy->src, dmaCopy->dst, dmaCopy->size);
		}
		else {
			halDma2D_StartA(dmaCopy->src, dmaCopy->dst,
				dmaCopy->size, dmaCopy->width,
				dmaCopy->srcStride, dmaCopy->dstStride);
		}
	}
#else
	void startFunc(MyDmaTask* dmaCopy) {
		if (dmaCopy->type == MSD_DMA) {
			halCopyRISC(dmaCopy->src, dmaCopy->dst, dmaCopy->size);
		}
		else {
			int* src = (int*)dmaCopy->src;
			int* dst = (int*)dmaCopy->dst;
			for (int i = 0; i < dmaCopy->size; i += dmaCopy->width) {
				halCopyRISC(src, dst, dmaCopy->width);
				src += dmaCopy->srcStride;
				dst += dmaCopy->dstStride;
			}
		}
		if (currentChannel->ptrTail()->callback != 0) {
			currentChannel->ptrTail()->callback();
		}
		currentChannel->incTail();
		isWorking = false;
	}
#endif
};

void msdInit();
unsigned int msdAdd(const void* src, void* dst, int size, int priority = MSD_NUM_CHANNELS - 1);
unsigned int msdAdd2D(const void* src, void* dst, unsigned size, unsigned width, unsigned srcStride32, unsigned dstStride32, int priority = MSD_NUM_CHANNELS - 1);
void msdWaitDma();
void msdWaitDma(int channel);
bool msdGetStatusCopy(int index, int priority = MSD_NUM_CHANNELS - 1);

unsigned int msdAdd(MyDmaTask &task, int priority = MSD_NUM_CHANNELS - 1);


#endif