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
	msdCallback callback;
};

void msdAddImage(ImageBuffer* buffer, ImageSegment* segment, int startIndex, int step);
void msdAdd(const void* src, void* dst, int size);
void msdAdd2D(const void* src, void* dst, unsigned size, unsigned width, unsigned srcStride32, unsigned dstStride32);
void msdStartCopy();
void msdWaitDma();
bool msdGetStatusCopy(int index);


#endif