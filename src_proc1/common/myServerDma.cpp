#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include <nmpp.h>
#include <stdio.h>

struct ListCopy{
	void* src;
	void* dst;
	int size;
	volatile bool status;
};

SECTION(".data_demo3d") ListCopy list[100];

SECTION(".data_demo3d") volatile bool isBusy = false;
SECTION(".data_demo3d") volatile bool listCopy = false;
int countList = 0;
int currentIndex;


SECTION(".text_demo3d") void cbUpdate() {
	if (listCopy) {
		list[currentIndex].status = true;
		currentIndex++;
		if (currentIndex < countList) {
			halDmaStartA(list[currentIndex].src, list[currentIndex].dst, list[currentIndex].size);
		}
		else {
			isBusy = false;
			listCopy = false;
		}
	}
	else {
		isBusy = false;
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
		list[listSize].status = false;
		size -= segment->size;
		setPoint += segment->size;
		listSize += step;
		countList++;
	}
}

SECTION(".text_demo3d") void msdAdd(void* src, void* dst, int size) {
	list[countList].src = src;
	list[countList].dst = dst;
	list[countList].size = size;
	list[countList].status = false;
	countList++;
}

SECTION(".text_demo3d") bool msdGetStatusCopy(int index) {
	return list[index].status;
}

SECTION(".text_demo3d") void msdStartCopy() {
	currentIndex = 0;
	isBusy = true;
	listCopy = true;
	halDmaSetCallback((DmaCallback)cbUpdate);
	halDmaStartA(list[currentIndex].src, list[currentIndex].dst, list[currentIndex].size);
}

SECTION(".text_demo3d") void msdWaitDma() {
	while (isBusy);
	countList = 0;
}

SECTION(".text_demo3d") void initMatrixCopy(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32) {
	msdWaitDma();
	isBusy = true;
	halDmaSetCallback((DmaCallback)cbUpdate);
	halDma2D_StartA(src, dst, size32, width, srcStride32, dstStride32);
}
