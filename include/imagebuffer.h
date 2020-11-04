#ifndef __IMAGE_BUFFER_H__
#define __IMAGE_BUFFER_H__
#include "nmpp.h"
#include "nmgl.h"
#include "myserverdma.h"

#ifndef __NM__
typedef void DepthCore32(nm32s &buffZ, nm32s &trianSrcZ, nm32s &trianDstZ);
typedef void DepthCore16(nm16s &buffZ, nm16s &trianSrcZ, nm16s &trianDstZ);
#else
typedef void DepthCore32();
typedef void DepthCore16();
#endif // !__NM__

typedef int ImageRGB8888[WIDTH_IMAGE * HEIGHT_IMAGE];
typedef int DepthImage32[WIDTH_IMAGE * HEIGHT_IMAGE];
typedef int ImageRGB565[WIDTH_IMAGE * HEIGHT_IMAGE / 2];
typedef int DepthImage16[WIDTH_IMAGE * HEIGHT_IMAGE / 2];

//при изменении типа отрисовываемого изображения, следует так же поменять
//код в функции readMask
#define OUTPUT_IMAGE_RGB8888

#ifdef OUTPUT_IMAGE_RGB8888
#define NMGL_IMAGE ImageRGB8888
#define IMAGE_BUFFER_CLASS ImageBufferRgb8888
#define PIXELS_IN_LONG 2
#define PIXELS_IN_LONG_MINUS_ONE 1
#define RED_COEFF 255.0f
#define BLUE_COEFF 255.0f
#define GREEN_COEFF 255.0f
#define ALPHA_COEFF 255.0f
#define ZBUFF_MAX 0x7FFFFFFF
typedef DepthImage32 DepthImage;
typedef HalRingBufferData<ImageRGB8888, COUNT_IMAGE_BUFFER> ImageData;
typedef HalRingBufferConnector<ImageRGB8888, COUNT_IMAGE_BUFFER> ImageConnector;

#define COMMON_DRAW_TYPE nm32s
#define MUL_Z_FUNC mMulCVxN_2s32s
#define MUL_C_FUNC mMulCVxN_2s_RGB8888
#define DEPTH_FUNC depthTest32
#define MASK_FUNC mMaskVxN_32s
#endif

#ifdef OUTPUT_IMAGE_RGB565
#define NMGL_IMAGE ImageRGB565
#define IMAGE_BUFFER_CLASS ImageBufferRgb565
#define PIXELS_IN_LONG 4
#define PIXELS_IN_LONG_MINUS_ONE 3
#define RED_COEFF 31.0f
#define GREEN_COEFF 63.0f
#define BLUE_COEFF 31.0f
#define ALPHA_COEFF 255.0f
#define ZBUFF_MAX 0x7FFF
typedef DepthImage16 DepthImage;
typedef HalRingBufferData<ImageRGB565, COUNT_IMAGE_BUFFER> ImageData;
typedef HalRingBufferConnector<ImageRGB565, COUNT_IMAGE_BUFFER> ImageConnector;

#define COMMON_DRAW_TYPE nm16s
#define MUL_Z_FUNC mMulCVxN_2s16s
#define MUL_C_FUNC mMulCVxN_2s_RGB565
#define DEPTH_FUNC depthTest16
#define MASK_FUNC mMaskVxN_16s

#endif

class ImageBufferRgb8888 {
private:
	MyDmaTask task;
	int mWidth;
	int mHeight;
	int mSize;
	
public:
	void* mData;
	int mClearValue;
	void* mCursor;

	void init(void* data, int width, int height) {
		mData = data;
		mWidth = width;
		mHeight = height;
		mSize = width * height;
		mCursor = data;
	}

	inline int getWidth() {
		return mWidth;
	}

	inline int getHeight() {
		return mHeight;
	}

	inline int getSize() {
		return mSize;
	}

	inline void clear() {
		nmppsSet_32s((int*)mData, mClearValue, mSize);
	}

	inline void clearColor(int red, int green, int blue, int alpha) {
		mClearValue = blue;
		mClearValue |= (green << 8);
		mClearValue |= (red << 16);
		mClearValue |= (alpha << 24);
	}

	inline void *setCursor(int x, int y) {
		mCursor = nmppsAddr_32s((nm32s*)mData, y * mWidth + x);
		return mCursor;
	}

	void pushWindow(ImageBufferRgb8888 &image, int width, int height, msdCallback callback = 0) {
		task.src = mCursor;
		task.dst = image.mCursor;
		task.size = width * height;
		task.width = width;
		task.srcStride = mWidth;
		task.dstStride = image.getWidth();
		task.type = MSD_DMA_2D;
		task.callback = callback;
		msdAdd(task, 1);
	}

	void popWindow(ImageBufferRgb8888 &image, int width, int height, msdCallback callback = 0) {
		task.src = image.mCursor;
		task.dst = mCursor;
		task.size = width * height;
		task.width = width;
		task.srcStride = image.getWidth();
		task.dstStride = mWidth;
		task.type = MSD_DMA_2D;
		task.callback = callback;
		msdAdd(task, 1);
	}
};

class ImageBufferRgb565{
private:
	MyDmaTask task;
	int mWidth;
	int mHeight;
	int mSize;

public:
	void* mData;
	int mClearValue;
	void* mCursor;

	void init(void* data, int width, int height) {
		mData = data;
		mWidth = width;
		mHeight = height;
		mSize = width * height;
		mCursor = data;
	}

	inline int getWidth() {
		return mWidth;
	}

	inline int getHeight() {
		return mHeight;
	}

	inline int getSize() {
		return mSize;
	}

	inline void clear() {
		nmppsSet_16s((nm16s*)mData, mClearValue, mSize);
	}

	inline void clearColor(int red, int green, int blue, int alpha) {
		mClearValue = blue & 0x1F;
		mClearValue |= ((green & 0x3F) << 5);
		mClearValue |= ((red & 0x1F) << 11);
	}

	inline void *setCursor(int x, int y) {
		mCursor = nmppsAddr_16s((nm16s*)mData, y * mWidth + x);
		return mCursor;
	}

	void pushWindow(ImageBufferRgb565 &image, int width, int height, msdCallback callback = 0) {
		task.src = mCursor;
		task.dst = image.mCursor;
		task.size = width * height / 2;
		task.width = width / 2;
		task.srcStride = mWidth / 2;
		task.dstStride = image.getWidth() / 2;
		task.type = MSD_DMA_2D;
		task.callback = callback;
		msdAdd(task, 1);
	}

	void popWindow(ImageBufferRgb565 &image, int width, int height, msdCallback callback = 0) {
		task.src = image.mCursor;
		task.dst = mCursor;
		task.size = width * height / 2;
		task.width = width / 2;
		task.srcStride = image.getWidth() / 2;
		task.dstStride = mWidth / 2;
		task.type = MSD_DMA_2D;
		task.callback = callback;
		msdAdd(task, 1);
	}

};

class DepthBuffer : public IMAGE_BUFFER_CLASS {
private:
	bool maskEnabled;
	int mode;
	int dummy;

	void update();

public:
	bool enabled;

	DepthBuffer() {
		enabled = NMGL_FALSE;
		maskEnabled = NMGL_TRUE;
		mode = NMGL_LESS;
		mClearValue = ZBUFF_MAX;
	}

	void setEnabledMask(bool flag) {
		maskEnabled = flag;
		update();
	}

	void setMode(int depthMode) {
		mode = depthMode;
		update();
	}

};


#endif
