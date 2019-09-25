#ifndef __IMAGE_BUFFER_H__
#define __IMAGE_BUFFER_H__

#include "ringbuffer.h"

typedef void(*FuncClearImage)(void* dst, int value, int size);

class ImageBuffer{
private:
	int width;
	int height;
public:
	HalRingBuffer ringbuffer;
	int clearValue;
	FuncClearImage funcClear;

	void set(void* imageArray, int widthImage, int heightImage, int count, FuncClearImage funcClearImage) {
		width = widthImage;
		height = heightImage;
		halRingBufferInit(&ringbuffer, imageArray, width * height, count, 0, 0, 0);
		funcClear = funcClearImage;
	}

	void* top() {
		return halRingBufferHead(&ringbuffer);
	}

	void next() {
		while (halRingBufferIsFull(&ringbuffer));
		ringbuffer.head++;
	}
	
	int getWidth(){
		return width;
	}
	
	int getHeight(){
		return height;
	}
	
	int getSize(){
		return ringbuffer.size;
	}

	void clearImage() {
		funcClear(top(), clearValue, getSize());
	}

};


#endif
