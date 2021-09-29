#ifndef __IMAGE_BUFFER_H__
#define __IMAGE_BUFFER_H__
#include "nmpp.h"
#include "nmgl.h"

#ifndef __NM__
typedef void DepthCore32(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ);
#else
typedef void DepthCore32();
#endif // !__NM__


#define PIXELS_IN_LONG 2
#define PIXELS_IN_LONG_MINUS_ONE 1
#define RED_COEFF 255.0f
#define BLUE_COEFF 255.0f
#define GREEN_COEFF 255.0f
#define ALPHA_COEFF 255.0f
#define ZBUFF_MAX 0x7FFFFFFF

#define IMAGE_BUFFER_BACK 0
#define IMAGE_BUFFER_FRONT 1
#define DEPTH_BUFFER 2

struct NMGL_Buffer {
#ifdef __GNUC__
	void* data;
#else
	int data;
#endif
	int enabled;
	int mask;
	int mode;
};

class NMGL_Framebuffer {
private:
	int mWidth;
	int mHeight;
	int mSize;
	int dummy;
public:
	volatile unsigned long long head;
	volatile unsigned long long tail;
	NMGL_Buffer imageBuffer;		/// буфер, который в данный момент отрисовывается (GL_LEFT_BACK)
	NMGL_Buffer imageBufferFront;	/// буфер, который отображается в данный момент (GL_LEFT_FRONT)
	NMGL_Buffer depthBuffer;
	NMGL_Buffer stencilBuffer;

	NMGL_Framebuffer() {
		mWidth = 0xdeadb00f;
		mHeight = 0xdeadb00f;
		head = 0;
		tail = 0;
		imageBuffer.enabled = NMGL_TRUE;
		imageBuffer.mask = 0xFFFFFFFF;
		imageBufferFront.enabled = NMGL_TRUE;
		imageBufferFront.mask = 0xFFFFFFFF;

		depthBuffer.enabled = NMGL_FALSE;
		depthBuffer.mask = NMGL_TRUE;
		depthBuffer.mode = NMGL_LESS;

		stencilBuffer.enabled = NMGL_FALSE;
		stencilBuffer.mask = 0xFFFFFFFF;
	}

	void setSize(int width, int height) {
		mWidth = width;
		mHeight = height;
		mSize = width * height;
	}

	int getSize() {
		return mSize;
	}
	int getWidth() {
		return mWidth;
	}
	int getHeight() {
		return mHeight;
	}
};


#endif
