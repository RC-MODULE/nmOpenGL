#include "pattern.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "stdio.h"
#include "windows.h"
#include <iostream>
#include <fstream>
using namespace std;

#define abs(a) (((a) < 0) ? -(a) : (a))

static void setPixel(unsigned int* dst, int x, int y, unsigned int color);
//static void fillRow(unsigned int* dst, int x1, int x2, int y, unsigned int color);
static void drawLine(unsigned int* dst, int x1, int y1, int x2, int y2, unsigned int color);
static void fillSide(unsigned int* dst, int x1, int y1, int x2, int y2, unsigned int color, int leftSide);
static void fillCircle(unsigned int* dst, int x0, int y0, int radius, unsigned int color);
static void createLineTable(int* table);
static void createFillTable(int* table);


void fillPtrnsInit(unsigned int* dst, int* table_dydx, unsigned int color) {
	int cntRight = 0;
	int cntLeft = FILL_PATTERNS_AMOUNT / 2;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;


	for (int i = 0; i < FILL_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}

	int r = 2 * max(WIDTH_PTRN, HEIGHT_PTRN);
	float stepA = M_PI / AMOUNT_ANGLES;
	for (float angle = 0; angle < AMOUNT_ANGLES/4; angle++) {
		for (int off = 0; off < OFFSETS; off++, cntRight++, cntLeft++) {
			if (angle == 0) {
				for (int x = off; x < WIDTH_PTRN; x++) {
					drawLine(dst + cntRight * size, x, 0, x, HEIGHT_PTRN, color);
				}
				for (int x = 0; x < off; x++) {
					drawLine(dst + cntLeft * size, x, 0, x, HEIGHT_PTRN, color);
				}
			}
			else {
				fillSide(dst + cntRight * size, off, 0, off + r * cos(angle * stepA), r * sin(angle * stepA), color, 1);
				fillSide(dst + cntLeft  * size, off, 0, off + r * cos(angle * stepA), r * sin(angle * stepA), color, 0);
			}
		}
	}
	for (float angle = AMOUNT_ANGLES / 4; angle < AMOUNT_ANGLES / 2; angle++) {
		for (int off = 0; off < OFFSETS; off++, cntRight++, cntLeft++) {
			fillSide(dst + cntRight * size, off, 0, off + r * cos(angle * stepA), r * sin(angle * stepA), color, 1);
			fillSide(dst + cntLeft  * size, off, 0, off + r * cos(angle * stepA), r * sin(angle * stepA), color, 0);
		}
	}

	for (float angle = AMOUNT_ANGLES / 2; angle < 3 * AMOUNT_ANGLES / 4; angle++) {
		for (int off = 0; off < OFFSETS; off++, cntRight++, cntLeft++) {
			if (angle == AMOUNT_ANGLES/2) {
				for (int x = off; x < WIDTH_PTRN; x++) {
					drawLine(dst + cntRight * size, x, 0, x, HEIGHT_PTRN, color);
				}
				for (int x = 0; x < off; x++) {
					drawLine(dst + cntLeft * size, x, 0, x, HEIGHT_PTRN, color);
				}
			}
			else {
				fillSide(dst + cntRight * size, off, 0, off + r * cos(angle * stepA), r * sin(angle * stepA), color, 1);
				fillSide(dst + cntLeft  * size, off, 0, off + r * cos(angle * stepA), r * sin(angle * stepA), color, 0);
			}
			
		}
	}

	for (float angle = 3 * AMOUNT_ANGLES / 4; angle < AMOUNT_ANGLES; angle++) {
		for (int off = 0; off < OFFSETS; off++, cntRight++, cntLeft++) {
			fillSide(dst + cntRight * size, off, 0, off + r * cos(angle * stepA), r * sin(angle * stepA), color, 1);
			fillSide(dst + cntLeft  * size, off, 0, off + r * cos(angle * stepA), r * sin(angle * stepA), color, 0);
		}
	}
	
	//create_tabl_dydx(dst, table_dydx, WIDTH_PTRN, HEIGHT_PTRN);
	createFillTable(table_dydx);
}

static void createFillTable(int* table) {
	for (int y = 0; y < HEIGHT_TABLE; y++) {
		for (int x = -WIDTH_TABLE / 2; x < 0; x++) {
			float number = floor(atan2(y, x) * AMOUNT_ANGLES / M_PI);
			table[y * WIDTH_TABLE + x + WIDTH_TABLE / 2] = number * OFFSETS;
		}
		for (int x = 0; x < WIDTH_TABLE / 2; x++) {
			float number = ceil(atan2(y, x) * AMOUNT_ANGLES / M_PI);
			table[y * WIDTH_TABLE + x + WIDTH_TABLE / 2] = number * OFFSETS;
		}
	}
	
}

void linePtrnsInit(unsigned int* dst, int* table, unsigned int color) {
	int cnt = 0;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;
	for (int i = 0; i < LINE_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}
	float stepA = M_PI / AMOUNT_ANGLES;
	int r = 2 * max(WIDTH_PTRN, HEIGHT_PTRN);
	for (float angle = 0; angle < AMOUNT_ANGLES / 2; angle++, cnt++) {
		drawLine(dst + cnt * size, 0, 0, r * cos(angle * stepA), r * sin(angle * stepA), color);
	}

	for (float angle = AMOUNT_ANGLES / 2; angle < AMOUNT_ANGLES; angle++, cnt++) {
		drawLine(dst + cnt * size, WIDTH_PTRN, 0, WIDTH_PTRN - 1 + r * cos(angle * stepA), r * sin(angle * stepA), color);
	}

	createLineTable(table);
	
}


static void createLineTable(int* table) {
	for (int i = 0; i < SIZE_TABLE; i++) {
		table[i] = 0;
	}
	for (int y = 0; y < HEIGHT_TABLE; y++) {
		for (int x = -WIDTH_TABLE / 2; x < WIDTH_TABLE / 2; x++) {
			float angle = round(atan2(y, x) * AMOUNT_ANGLES / M_PI);
			table[y * WIDTH_TABLE + x + WIDTH_TABLE / 2] = angle;
		}
	}
}

void pointPtrnsInit(unsigned int* dst, unsigned int color) {
	int cnt = 0;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;
	for (int i = 0; i < POINT_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}
	for (int d = 1; d < POINT_PATTERNS_AMOUNT; d++, cnt++) {
		unsigned int* tmpDst = dst + cnt * size;
		for (int y = 0; y < d; y++) {
			for (int x = 0; x < d; x++) {
				tmpDst[y * WIDTH_PTRN + x] = color;
			}
		}
	}
}


static void setPixel(unsigned int* dst, int x, int y, unsigned int color) {
	if(x < 0 || x >= WIDTH_PTRN ||
		y < 0 || y >= HEIGHT_PTRN){
		return;
	}
	dst[y * WIDTH_PTRN + x] = color;
}

static void fillRow(unsigned int* dst, int x1, int x2, int y, unsigned int color) {
	if (y >= 0 && y < HEIGHT_PTRN) {
		for (int x = x1; x < x2; x++) {
			if (x >= 0 && x < WIDTH_PTRN) {
				dst[y * WIDTH_PTRN + x] = color;
			}
		}
	}
}


static void drawLine(unsigned int* dst, int x1, int y1, int x2, int y2, unsigned int color) {
	int deltaX = abs(x2 - x1);
	int deltaY = abs(y2 - y1);
	int signX = x1 < x2 ? 1 : -1;
	int signY = y1 < y2 ? 1 : -1;

	if (abs(deltaX) > abs(deltaY)) {
		int deltaError = deltaY;
		int error = 0;
		while (x1 != x2) {
			setPixel(dst, x1, y1, color);
			error += deltaError;
			if (error >= deltaX) {
				y1 += signY;
				error -= deltaX;
			}
			x1 += signX;
		}
	}
	else {
		int deltaError = deltaX;
		int error = 0;
		while (y1 != y2) {
			setPixel(dst, x1, y1, color);
			error += deltaError;
			if (error >= deltaY) {
				x1 += signX;
				error -= deltaY;
			}
			y1 += signY;
		}
	}
}

static void fillSide(unsigned int* dst, int x1, int y1, int x2, int y2, unsigned int color, int topLeftSide) {
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	int dx = (deltaX > 0) ? 1 : -1;
	int dy = (deltaY > 0) ? 1 : -1;
	double k, err;
	if (abs(deltaX) >= abs(deltaY)) {
		k = 2 * (double)fabs(deltaY) / fabs(deltaX);
		err = 0;
		int y = y1;
		for (int x = x1; x != x2; x += dx) {
			if (err > 1) {
				y += dy;
				err -= 2;
			}
			err += k;
			if(topLeftSide){
				for(int i = x; i < WIDTH_PTRN; i++){
					setPixel(dst, i, y, color);
				}
			} else {
				for(int i = 0; i < x + 1; i++){
					setPixel(dst, i, y, color);
				}
			}
		}
		if (topLeftSide == 1 && deltaX < 0 ||
			topLeftSide == 0 && deltaX > 0) {
			for (int y = y2; y < HEIGHT_PTRN; y++) {
				for (int x = 0; x < WIDTH_PTRN; x++) {
					setPixel(dst, x, y, color);
				}
			}
		}
	}
	else {
		k = 2 * (double)fabs(deltaX) / fabs(deltaY);
		err = 0;
		int x = x1;
		for (int y = y1; y != y2; y += dy) {
			if (err > 1) {
				x += dx;
				err -= 2;
			}
			err += k;
			if(topLeftSide){
				for(int i = x; i < WIDTH_PTRN; i++){
					setPixel(dst, i, y, color);
				}
			} else {
				for(int i = 0; i < x + 1; i++){
					setPixel(dst, i, y, color);
				}
			}
		}
		
	}
	/*float deltaY = (y2 - y1);
	float deltaX = (x2 - x1);
	float k = deltaY / deltaX;
	float b = y1 - k * x1;
	for (int y = 0; y < HEIGHT_PTRN; y++) {
		for (int x = 0; x < WIDTH_PTRN; x++) {
			if (fabs(deltaX) > fabs(deltaY)) {
				float yRefp = k * ((float)x) + b;
				float yRefs = k * ((float)x) + b;
				if (topLeftSide == 0 && (yRefp <= y + 0.5) ||
					topLeftSide == 1 && (yRefs >= y - 0.5)) {
					setPixel(dst, x, y, color);
				}
				else {
					setPixel(dst, x, y, 0);
				}
			}
			else {
				float xRefp = ((float)(y - b)) / k;
				float xRefs = ((float)(y - b)) / k;
				if (topLeftSide == 0 && (xRefp <= x + 0.5) ||
					topLeftSide == 1 && (xRefp >= x - 0.5)) {
					setPixel(dst, x, y, color);
				}
				else {
					setPixel(dst, x, y, 0);
				}
			}
		}
	}*/
}

static void fillCircle(unsigned int* dst, int x0, int y0, int radius, unsigned int color) {
	int x = 0;
	int y = radius;
	int delta = 1 - 2 * radius;
	int error = 0;
	while (y >= 0) {
		//setPixel(dst, x0 + x, y0 + y, color);
		//setPixel(dst, x0 + x, y0 - y, color);
		//setPixel(dst, x0 - x, y0 + y, color);
		//setPixel(dst, x0 - x, y0 - y, color);
		fillRow(dst, x0 - x, x0 + x, y0 - y, color);
		fillRow(dst, x0 - x, x0 + x, y0 + y, color);
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


