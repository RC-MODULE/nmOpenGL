#include "pattern.h"

#define PI 3.14159265359

#define abs(a) (((a) < 0) ? -(a) : (a))

static void setPixel(unsigned char* dst, int x, int y, int color) {
	if(x < 0 || x >= WIDTH_PTRN ||
		y < 0 || y >= HEIGHT_PTRN){
		return;
	}
	dst[y * WIDTH_PTRN + x] = color;
}


static void drawLine(unsigned char* dst, int x1, int y1, int x2, int y2, int color) {
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	//
	int error = deltaX - deltaY;
	//
	setPixel(dst, x2, y2, color);
	while (x1 != x2 || y1 != y2)
	{
		setPixel(dst, x1, y1, color);
		const int error2 = error * 2;
		//
		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}


void fillPtrnsInit(unsigned char* dst, int color) {
	
}

void linePtrnsInit(unsigned char* dst, int color) {
	int counter = 0;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;

	for (int i = 0; i < LINE_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}

	for (int x = 0; x < WIDTH_PTRN; x++) {
		drawLine(dst + counter++ * size, 0, 0, x, HEIGHT_PTRN - 1, color);
	}
	for (int y = HEIGHT_PTRN - 1; y >= 0; y--) {
		drawLine(dst + counter++ * size, 0, 0, WIDTH_PTRN - 1, y, color);
	}

	for (int x = 0; x < WIDTH_PTRN; x++) {
		drawLine(dst + counter++ * size, 0, HEIGHT_PTRN - 1, x, 0, color);
	}
	for (int y = 0; y < HEIGHT_PTRN; y++) {
		drawLine(dst + counter++ * size, 0, HEIGHT_PTRN - 1, WIDTH_PTRN - 1, y, color);
	}

	
}

void pointPtrnsInit(unsigned char* dst, int color) {

}