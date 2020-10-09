#include "pattern.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "stdio.h"
#include "windows.h"
#include <iostream>
#include <fstream>
using namespace std;
#define PI 3.14159265359

#define abs(a) (((a) < 0) ? -(a) : (a))

static void setPixel(unsigned char* dst, int x, int y, unsigned char color);
//static void fillRow(unsigned char* dst, int x1, int x2, int y, unsigned char color);
static void drawLine(unsigned char* dst, int x1, int y1, int x2, int y2, unsigned char color);
static void fillSide(unsigned char* dst, int x1, int y1, int x2, int y2, unsigned char color, int leftSide);
static void fillCircle(unsigned char* dst, int x0, int y0, int radius, unsigned char color);
static void createLineTable(int* table);
static void createFillTable(int* table);


void fillPtrnsInit(unsigned char* dst, int* table_dydx, unsigned char color) {
	int cntRight = 0;
	int cntLeft = FILL_PATTERNS_AMOUNT / 2;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;


	for (int i = 0; i < FILL_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}

	int r = 2 * max(WIDTH_PTRN, HEIGHT_PTRN);
	float stepA = M_PI / AMOUNT_ANGLES;
	for (float angle = 0; angle < AMOUNT_ANGLES/2; angle++) {
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
				fillSide(dst + cntRight * size, off, 0, off + r * cos(angle * M_PI / 180), r * sin(angle * M_PI / 180), color, 1);
				fillSide(dst + cntLeft  * size, off, 0, off + r * cos(angle * M_PI / 180), r * sin(angle * M_PI / 180), color, 0);
			}
		}
	}

	for (float angle = AMOUNT_ANGLES/2; angle < AMOUNT_ANGLES; angle++) {
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
				fillSide(dst + cntRight * size, off, 0, off + r * cos(angle * M_PI / 180), r * sin(angle * M_PI / 180), color, 0);
				fillSide(dst + cntLeft  * size, off, 0, off + r * cos(angle * M_PI / 180), r * sin(angle * M_PI / 180), color, 1);
			}
			
		}
	}
	
	//create_tabl_dydx(dst, table_dydx, WIDTH_PTRN, HEIGHT_PTRN);
	createFillTable(table_dydx);
}

static void createFillTable(int* table) {
	for (int y = 0; y < HEIGHT_TABLE; y++) {
		for (int x = -WIDTH_TABLE / 2; x < WIDTH_TABLE / 2; x++) {
			double k = (double)y / (double)x;
			float angle = round(atan2(y, x) * 180 / M_PI);
			table[y * WIDTH_TABLE + x + WIDTH_TABLE / 2] = angle * OFFSETS;
		}
	}
	
}

void linePtrnsInit(unsigned char* dst, int* table, unsigned char color) {
	int cnt = 0;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;
	for (int i = 0; i < LINE_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}

	int r = 2 * max(WIDTH_PTRN, HEIGHT_PTRN);
	float stepA = M_PI / AMOUNT_ANGLES;
	for (float angle = 0; angle < AMOUNT_ANGLES / 2; angle++, cnt++) {
		drawLine(dst + cnt * size, 0, 0, r * cos(angle * M_PI / 180), r * sin(angle * M_PI / 180), color);
	}

	for (float angle = AMOUNT_ANGLES / 2; angle < AMOUNT_ANGLES; angle++, cnt++) {
		drawLine(dst + cnt * size, WIDTH_PTRN - 1, 0, WIDTH_PTRN - 1 + r * cos(angle * M_PI / 180), r * sin(angle * M_PI / 180), color);
	}

	/*for (int y = 0; y < HEIGHT_PTRN; y++, cnt++) {
		drawLine(dst + cnt * size, WIDTH_PTRN, 0, 0, y, color);
	}
	for (int x = 0; x <= WIDTH_PTRN; x++, cnt++) {
		drawLine(dst + cnt * size, WIDTH_PTRN, 0, x, HEIGHT_PTRN, color);
	}


	for (int x = 0; x < WIDTH_PTRN; x++, cnt++) {
		drawLine(dst + cnt * size, 0, 0, x, HEIGHT_PTRN, color);
	}
	for (int y = HEIGHT_PTRN - 1; y >= 0; y--, cnt++) {
		drawLine(dst + cnt * size, 0, 0, WIDTH_PTRN, y, color);
	}*/


	createLineTable(table);
	
}


static void createLineTable(int* table) {
	for (int i = 0; i < SIZE_TABLE; i++) {
		table[i] = 0;
	}
	for (int y = 0; y < HEIGHT_TABLE; y++) {
		for (int x = -WIDTH_TABLE / 2; x < WIDTH_TABLE / 2; x++) {
			double k = (double)y / (double)x;
			float angle = round(atan2(y, x) * 180 / M_PI);
			table[y * WIDTH_TABLE + x + WIDTH_TABLE / 2] = angle;
		}
	}

	/*for (int x = -WIDTH_PTRN; x < WIDTH_PTRN; x++) {
		table[x + WIDTH_PTRN] = 0;
	}
	for (int y = 1; y <= HEIGHT_PTRN; y++) {
		for (int x = -WIDTH_PTRN; x < WIDTH_PTRN; x++) {
			if (x == 0) {
				table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = LINE_PATTERNS_AMOUNT / 2 + 1;
				continue;
			}
			double k = (double)y / (double)x;
			int signX = (x < 0) ? -1 : 1;
			if (x < 0) {
				if (abs(x) < y) {
					int resX = -round(HEIGHT_PTRN / k);
					table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = 64 - resX;
				}
				else {
					int resY = -round(WIDTH_PTRN * k);
					table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = resY;
				}
			}
			else {
				if (abs(x) < y) {
					int resX = round(HEIGHT_PTRN / k);
					table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = resX + LINE_PATTERNS_AMOUNT / 2;
				}
				else {
					int resY = round(WIDTH_PTRN * k);
					table[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN] = 64 - resY + LINE_PATTERNS_AMOUNT / 2;
				}
			}
		}
	}*/
}

void pointPtrnsInit(unsigned char* dst, unsigned char color) {
	int cnt = 0;
	const int size = WIDTH_PTRN * HEIGHT_PTRN;
	for (int i = 0; i < POINT_PATTERNS_AMOUNT * size; i++) {
		dst[i] = 0;
	}
	for (int d = 1; d < POINT_PATTERNS_AMOUNT; d++, cnt++) {
		unsigned char* tmpDst = dst + cnt * size;
		for (int y = 0; y < d; y++) {
			for (int x = 0; x < d; x++) {
				tmpDst[y * WIDTH_PTRN + x] = color;
			}
		}
	}
}


static void setPixel(unsigned char* dst, int x, int y, unsigned char color) {
	if(x < 0 || x >= WIDTH_PTRN ||
		y < 0 || y >= HEIGHT_PTRN){
		return;
	}
	dst[y * WIDTH_PTRN + x] = color;
}

static void fillRow(unsigned char* dst, int x1, int x2, int y, unsigned char color) {
	if (y >= 0 && y < HEIGHT_PTRN) {
		for (int x = x1; x < x2; x++) {
			if (x >= 0 && x < WIDTH_PTRN) {
				dst[y * WIDTH_PTRN + x] = color;
			}
		}
	}
}


static void drawLine(unsigned char* dst, int x1, int y1, int x2, int y2, unsigned char color) {
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

static void fillSide(unsigned char* dst, int x1, int y1, int x2, int y2, unsigned char color, int topLeftSide) {
	float deltaY = (y2 - y1);
	float deltaX = (x2 - x1);
	float k = deltaY / deltaX;
	float b = y1 - k * x1;
	for (int y = 0; y < HEIGHT_PTRN; y++) {
		for (int x = 0; x < WIDTH_PTRN; x++) {
			float yRefp = k * (float(x)) + b;
			float yRefs = k * (float(x)) + b;
			if (topLeftSide == 0 && (yRefp <= y) ||
				topLeftSide == 1 && (yRefs >= y)) {
				setPixel(dst, x, y, color);
			}
		}
	}
}

static void fillCircle(unsigned char* dst, int x0, int y0, int radius, unsigned char color) {
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


