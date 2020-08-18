#pragma once

#define MAX_SIDE_POLYGON 32
#define HEIGHT_PTRN   MAX_SIDE_POLYGON
#define WIDTH_PTRN    MAX_SIDE_POLYGON
#define SMALL_SIZE 	  16
#define SIZE_BANK	0x8000
#define OFFSETS 	  WIDTH_PTRN
#define AMOUNT_ANGLES (2*WIDTH_PTRN + 2*HEIGHT_PTRN)
#define FILL_PATTERNS_AMOUNT	  AMOUNT_ANGLES * OFFSETS * 2
#define LINE_PATTERNS_AMOUNT 	  AMOUNT_ANGLES
#define POINT_PATTERNS_AMOUNT 	  (MAX_SIDE_POLYGON / 2)
#define NPATTERNS 				  FILL_PATTERNS_AMOUNT


typedef int Pattern[WIDTH_PTRN * HEIGHT_PTRN / 16];

struct PatternsArray {
	Pattern ptrns[FILL_PATTERNS_AMOUNT];
	Pattern linePtrns[LINE_PATTERNS_AMOUNT];
	Pattern pointPtrns[POINT_PATTERNS_AMOUNT];
	int table_dydx[(2 * WIDTH_PTRN) * (HEIGHT_PTRN + 2)];
	int lineTable[(2 * WIDTH_PTRN) * (HEIGHT_PTRN + 2)];
};

void fillPtrnsInit(unsigned char* dst, unsigned char color);
void linePtrnsInit(unsigned char* dst, unsigned char  color);
void pointPtrnsInit(unsigned char* dst, unsigned char color);

void hostCreatePatterns(PatternsArray* patterns);