#ifndef __IMAGE_BUFFER_H__
#define __IMAGE_BUFFER_H__



class ImageBuffer{
public:
	int width;
	int height;
	void* data;
	int clearValue;

	ImageBuffer() {
		clearValue = 0;
	}

	void init(void* imageAddr, int widthImage, int heightImage) {
		width = widthImage;
		height = heightImage;
		data = imageAddr;
	}
};


inline void copySubImage(ImageBuffer &srcImage, int srcX0, int srcY0, ImageBuffer &dstImage, int dstX0, int dstY0, int width, int height) {
	nm32s* src = (nm32s*)nmppsAddr_32s((nm32s*)srcImage.data, srcY0 * srcImage.width + srcX0);
	nm32s* dst = (nm32s*)nmppsAddr_32s((nm32s*)dstImage.data, dstY0 * dstImage.width + dstX0);
	nmppmCopy_32s(src, srcImage.width, dst, dstImage.width, height, width);
}

#endif
