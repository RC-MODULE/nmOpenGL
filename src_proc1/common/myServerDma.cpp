#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include <nmpp.h>
#include "myserverdma.h"


SECTION(".data_demo3d") MSD_DmaCopy list[100];

SECTION(".data_demo3d") volatile bool isBusy = false;
int countList = 0;
int currentIndex;
bool isSimpleCopy = false;


SECTION(".text_demo3d") void cbUpdate() {
	if (isSimpleCopy) {
		isBusy = false;
		isSimpleCopy = false;
	}
	else {
		if (list[currentIndex].callback) {
			list[currentIndex].callback();
		}
		list[currentIndex].status = true;
		currentIndex++;
		if (currentIndex < countList) {
			switch (list[currentIndex].type)
			{
			case MSD_DMA:
				halDmaStartA(list[currentIndex].src, list[currentIndex].dst, list[currentIndex].size);
				break;
			case MSD_DMA_2D:
				halDma2D_StartA(list[currentIndex].src, list[currentIndex].dst,
					list[currentIndex].size, list[currentIndex].width,
					list[currentIndex].srcStride, list[currentIndex].dstStride);
				break;
			default:
				break;
			}

		}
		else {
			isBusy = false;
			countList = 0;
		}
	}
}


SECTION(".text_demo3d") void msdAddImage(ImageBuffer* buffer, ImageSegment* segment, int startIndex, int step) {
	int* setPoint = (int*)buffer->top();
	nmppsSet_32s((nm32s*)segment->data, buffer->clearValue, segment->size);
	int size = buffer->getSize();
	int listSize = startIndex;
	while(size > 0) {
		list[listSize].src = segment->data;
		list[listSize].dst = setPoint;
		list[listSize].size = MIN(size, segment->size);
		list[listSize].type = MSD_DMA;
		list[listSize].status = false;
		size -= segment->size;
		setPoint += segment->size;
		listSize += step;
		countList++;
	}
}

SECTION(".text_demo3d") void msdAdd(const void* src, void* dst, int size) {
	list[countList].src = src;
	list[countList].dst = dst;
	list[countList].size = size;
	list[countList].status = false;
	list[countList].type = MSD_DMA;
	list[countList].callback = 0;
	countList++;
}

SECTION(".text_demo3d") void msdAdd2D(const void* src, void* dst, unsigned size, unsigned width, unsigned srcStride32, unsigned dstStride32) {
	list[countList].src = src;
	list[countList].dst = dst;
	list[countList].size = size;
	list[countList].status = false;
	list[countList].type = MSD_DMA_2D;
	list[countList].width = width;
	list[countList].srcStride = srcStride32;
	list[countList].dstStride = dstStride32;
	list[countList].callback = 0;
	countList++;
}

SECTION(".text_demo3d") bool msdGetStatusCopy(int index) {
	return list[index].status;
}

SECTION(".text_demo3d") void msdStartCopy() {
	currentIndex = 0;
	halDmaSetCallback((DmaCallback)cbUpdate);
	if (countList) {
		isBusy = true;
		switch (list[currentIndex].type)
		{
		case MSD_DMA:
			halDmaStartA(list[currentIndex].src, list[currentIndex].dst, list[currentIndex].size);
			break;
		case MSD_DMA_2D:
			halDma2D_StartA(list[currentIndex].src, list[currentIndex].dst,
				list[currentIndex].size, list[currentIndex].width,
				list[currentIndex].srcStride, list[currentIndex].dstStride);
			break;
		default:
			break;
		}
	}
}

SECTION(".text_demo3d") void msdSimpleCopy(const void* src, void* dst, int size) {
	halDmaSetCallback((DmaCallback)cbUpdate);
	isSimpleCopy = true;
	isBusy = true;
	halDmaStart(src, dst, size);
}


SECTION(".text_demo3d") void msdWaitDma() {
	volatile int a = 0;
	while (isBusy) {
		a++;
	}
}


