#include "pattern.h"
#include "math.h"

#define PI 3.14159265359

#define abs(a) (((a) < 0) ? -(a) : (a))

static void ptrnSetPixel(unsigned char* dst, int x, int y, unsigned char color);
static void ptrnFillRow(unsigned char* dst, int x1, int x2, int y, unsigned char color);
static void ptrnDrawLine(unsigned char* dst, int x1, int y1, int x2, int y2, unsigned char color);
static void ptrnFillSide(unsigned char* dst, int x1, int y1, int x2, int y2, unsigned char color, int leftSide);
static void ptrnFillCircle(unsigned char* dst, int x0, int y0, int radius, unsigned char color);


void fillPtrnsInit(unsigned char* dst, int* table_dydx, unsigned char color) {
	int cntRight = 0;
	int cntLeft = FILL_PATTERNS_AMOUNT / 2 - 1;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;

	for (int i = 0; i < FILL_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}
	for (int i = 0; i < 2 * size + 2 * WIDTH_PTRN; i++) {
		table_dydx[i] = 0;
	}

	for (int y = 0; y < HEIGHT_PTRN; y++) {
		for (int off = 0; off < OFFSETS; off++, cntRight++, cntLeft++) {
			ptrnFillSide(dst + cntRight * size, off, 0, -WIDTH_PTRN + off, y, color, 0);
			ptrnFillSide(dst + cntLeft  * size, off, 0, -WIDTH_PTRN + off, y, color, 1);
			if (y == 0) {
				for (int i = 0; i < size; i++) {
					dst[cntRight * size + i] = color;
					dst[cntLeft * size + i] = color;
				}
			}
		}
	}
	for (int x = -(WIDTH_PTRN - 1); x <= 0; x++) {
		for (int off = 0; off < OFFSETS; off++, cntRight++, cntLeft++) {
			ptrnFillSide(dst + cntRight * size, off, 0, x + off, HEIGHT_PTRN, color, 0);
			ptrnFillSide(dst + cntLeft  * size, off, 0, x + off, HEIGHT_PTRN, color, 1);
		}
	}
	for (int x = 0; x < WIDTH_PTRN; x++) {
		for (int off = 0; off < OFFSETS; off++, cntRight++, cntLeft++) {
			ptrnFillSide(dst + cntRight * size, off, 0, x + off, HEIGHT_PTRN, color, 0);
			ptrnFillSide(dst + cntLeft  * size, off, 0, x + off, HEIGHT_PTRN, color, 1);
		}
	}

	for (int y = HEIGHT_PTRN - 1; y >= 0; y--) {
		for (int off = 0; off < OFFSETS; off++, cntRight++, cntLeft++) {
			ptrnFillSide(dst + cntRight * size, off, 0, WIDTH_PTRN + off, y, color, 0);
			ptrnFillSide(dst + cntLeft  * size, off, 0, WIDTH_PTRN + off, y, color, 1);
		}
	}

	create_tabl_dydx(dst, table_dydx, WIDTH_PTRN, HEIGHT_PTRN);
}

void linePtrnsInit(unsigned char* dst, int* table, unsigned char color) {
	int cnt = 0;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;

//------------------------
	for (int i = 0; i < 2 * size + 2 * WIDTH_PTRN; i++) {
		table[i] = 0;
	}
	for (int i = 0; i < LINE_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}
//------------------------

	for (int x = 0; x < WIDTH_PTRN; x++, cnt++) {
		ptrnDrawLine(dst + cnt * size, 0, 0, x, HEIGHT_PTRN, color);
	}
	for (int y = HEIGHT_PTRN - 1; y >= 0; y--, cnt++) {
		ptrnDrawLine(dst + cnt * size, 0, 0, WIDTH_PTRN - 1, y, color);
	}


	for (int y = 0; y < HEIGHT_PTRN; y++, cnt++) {
		ptrnDrawLine(dst + cnt * size, WIDTH_PTRN, 0, 0, y, color);
	}
	for (int x = 0; x < WIDTH_PTRN; x++, cnt++) {
		ptrnDrawLine(dst + cnt * size, WIDTH_PTRN, 0, x, HEIGHT_PTRN, color);
	}

	for (int y = 0; y <= HEIGHT_PTRN; y++) {
		for (int x = -WIDTH_PTRN; x < WIDTH_PTRN; x++) {
			if (x != 0) {
				double k = (double)y / (double)x;
				int signX = (x < 0) ? -1 : 1;
				if (abs(x) < y) {
					if (x < 0) {
						int resX = -round(HEIGHT_PTRN / k);
						table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = 2 * 32 - resX + LINE_PATTERNS_AMOUNT / 2;
					}
					else {
						int resX = round(HEIGHT_PTRN / k);
						table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = resX;
					}
				}
				else {
					if (x < 0) {
						int resY = -round(WIDTH_PTRN * k);
						table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = resY + LINE_PATTERNS_AMOUNT / 2;
					}
					else {
						int resY = round(WIDTH_PTRN * k);
						table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = 2 * 32 - resY;
					}
				}
			}
			else {
				table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = 0;
			}
		}
	}
}

void pointPtrnsInit(unsigned char* dst, unsigned char color) {
	int cnt = 0;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;
	for (int i = 0; i < POINT_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}
	dst[0] = color; //point in one pixel
	cnt++;
	for (int r = 1; r < POINT_PATTERNS_AMOUNT; r++, cnt++) {
		unsigned char* tmpDst = dst + cnt * size;
		ptrnFillCircle(tmpDst, r, r, r, color);
	}
}


static void ptrnSetPixel(unsigned char* dst, int x, int y, unsigned char color) {
	if(x < 0 || x >= WIDTH_PTRN ||
		y < 0 || y >= HEIGHT_PTRN){
		return;
	}
	dst[y * WIDTH_PTRN + x] = color;
}

static void ptrnFillRow(unsigned char* dst, int x1, int x2, int y, unsigned char color) {
	if (y >= 0 && y < HEIGHT_PTRN) {
		for (int x = x1; x < x2; x++) {
			if (x >= 0 && x < WIDTH_PTRN) {
				dst[y * WIDTH_PTRN + x] = color;
			}
		}
	}
}


static void ptrnDrawLine(unsigned char* dst, int x1, int y1, int x2, int y2, unsigned char color) {
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	//
	int error = deltaX - deltaY;
	//
	ptrnSetPixel(dst, x2, y2, color);
	while (x1 != x2 || y1 != y2)
	{
		ptrnSetPixel(dst, x1, y1, color);
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

static void ptrnFillSide(unsigned char* dst, int x1, int y1, int x2, int y2, unsigned char color, int leftSide) {
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	//
	int error = deltaX - deltaY;
	//
	//ptrnSetPixel(dst, x2, y2, color);
	if (leftSide) 
		ptrnFillRow(dst, 0, x2 + 1, y2, color);
	else 
		ptrnFillRow(dst, x2, WIDTH_PTRN, y2, color);

	if (y1 == y2) {
		return;
	}
	while (x1 != x2 || y1 != y2)
	{
		//ptrnSetPixel(dst, x1, y1, color);
		if (leftSide) 
			ptrnFillRow(dst, 0, x1, y1, color);
		else 
			ptrnFillRow(dst, x1, WIDTH_PTRN, y1, color);
		
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
	//остаток изображения
	if (signX / signY < 0 && leftSide == 0 ||
		signX / signY > 0 && leftSide == 1) {
		for (int y = y1; y < HEIGHT_PTRN; y++) {
			ptrnFillRow(dst, 0, WIDTH_PTRN, y, color);
		}
	}
}

static void ptrnFillCircle(unsigned char* dst, int x0, int y0, int radius, unsigned char color) {
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while (y >= 0) {
		//ptrnSetPixel(dst, x0 + x, y0 + y, color);
		//ptrnSetPixel(dst, x0 + x, y0 - y, color);
		//ptrnSetPixel(dst, x0 - x, y0 + y, color);
		//ptrnSetPixel(dst, x0 - x, y0 - y, color);
		ptrnFillRow(dst, x0 - x, x0 + x, y0 - y, color);
		ptrnFillRow(dst, x0 - x, x0 + x, y0 + y, color);
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}


