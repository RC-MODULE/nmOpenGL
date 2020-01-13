#ifndef __IMAGE_BUFFER_H__
#define __IMAGE_BUFFER_H__



class ImageBuffer{
public:
	int width;
	int height;
	int size;
	void* data;
	ImageBuffer* subImage;
	int clearValue;

	ImageBuffer() {
		clearValue = 0;
	}

	void init(void* imageAddr, int widthImage, int heightImage) {
		width = widthImage;
		height = heightImage;
		setSize(widthImage, heightImage);
		data = imageAddr;
	}

	inline int getWidth() {
		return width;
	}

	inline int getHeight() {
		return height;
	}

	inline int getSize() {
		return size;
	}

	inline void setSize(int widthImage, int heightImage) {
		width = widthImage;
		height = heightImage;
		size = width * height;
	}
};

#endif
