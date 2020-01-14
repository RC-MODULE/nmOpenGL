#ifndef __IMAGE_BUFFER_H__
#define __IMAGE_BUFFER_H__

class ImageBuffer{
private:
	int width;
	int height;
	int size;
	int dummy;
public:
	void* data;
	int clearValue;

	ImageBuffer() {
		clearValue = 0;
	}

	void init(void* imageAddr, int widthImage, int heightImage) {
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





class DepthBuffer : public ImageBuffer {
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
		clearValue = ZBUFF_MAX;
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
