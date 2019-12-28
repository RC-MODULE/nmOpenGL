#ifndef __IMAGE_SEGMENT_H__
#define __IMAGE_SEGMENT_H__

#include "imagebuffer.h"

typedef void(*FuncMatrixCopy)(const void* src, void* dst, unsigned size32, unsigned width, unsigned srcStride32, unsigned dstStride32);


class ImageSegment{
private:
	int dummy;
public:
	void* data;
	int widthSeg;
	int heightSeg;
	int size;
	FuncMatrixCopy funcCopy;	

	void set(void* segmentAddr, int width, int height, FuncMatrixCopy funcMatrixCopy) {
		widthSeg = width;
		heightSeg = height;
		data = segmentAddr;
		funcCopy = funcMatrixCopy;
		size = widthSeg * heightSeg;
	}

	void push(ImageBuffer* image, int x0, int y0, int width, int height) {
		nm32s* dst = nmppsAddr_32s((int*)image->data, y0 * image->width + x0);
		funcCopy(data, dst, width * height, width, widthSeg, image->width);
	}

	void pop(ImageBuffer* image, int x0, int y0, int width, int height) {
		nm32s* src = nmppsAddr_32s((int*)image->data, y0 * image->width + x0);
		funcCopy(src, data, width * height, width, image->width, widthSeg);
	}

};



#endif
