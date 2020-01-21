#ifndef __MY_SERVER_DMA_H__
#define __MY_SERVER_DMA_H__ 


#define MSD_DMA 1
#define MSD_DMA_2D 2

typedef void(*msdCallback)();

struct MSD_DmaCopy{
	const void* src;
	void* dst;
	int size;
	int width;
	int srcStride;
	int dstStride;
	int type;
	volatile bool status;
};

void msdInit();
unsigned int msdAdd(const void* src, void* dst, int size);
unsigned int msdAdd2D(const void* src, void* dst, unsigned size, unsigned width, unsigned srcStride32, unsigned dstStride32);
void msdWaitDma();
bool msdGetStatusCopy(int index);


#endif