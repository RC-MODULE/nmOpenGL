#ifndef __IMAGE_BUFFER_H__
#define __IMAGE_BUFFER_H__

#include "ringbuffer.h"

class ImageBuffer{
private:
	int width;
	int height;
	HalRingBuffer ringbuffer;
	int dummy;
public:
	int clearValue;
	
	void set(void* imageArray, int widthImage, int heightImage, int count) {
		width = widthImage;
		height = heightImage;
		halRingBufferInit(&ringbuffer, imageArray, width * height, count, 0, 0, 0);
	}

	void* top() {
		return halRingBufferHead(&ringbuffer);
	}

	void next() {
		while (halRingBufferIsFull(&ringbuffer) || halRingBufferIsBusy(&ringbuffer));
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

	HalRingBuffer* getHalRingBuffer() {
		return &ringbuffer;
	}

};


#endif
