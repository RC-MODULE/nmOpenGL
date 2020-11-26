#pragma once

#define MAX_SIDE_POLYGON				32
#define HEIGHT_PTRN						MAX_SIDE_POLYGON
#define WIDTH_PTRN						MAX_SIDE_POLYGON
#define OFFSETS 						WIDTH_PTRN
//#define AMOUNT_ANGLES					(2*WIDTH_PTRN + 2*HEIGHT_PTRN)
#define AMOUNT_ANGLES					180
#define FILL_PATTERNS_AMOUNT			AMOUNT_ANGLES * OFFSETS * 2
#define LINE_PATTERNS_AMOUNT 			AMOUNT_ANGLES
#define POINT_PATTERNS_AMOUNT 			MAX_SIDE_POLYGON
#define WIDTH_TABLE						(2 * WIDTH_PTRN)
#define OFFSET_TABLE					(WIDTH_TABLE / 2)
#define HEIGHT_TABLE					(HEIGHT_PTRN + 1)	
#define SIZE_TABLE 						WIDTH_TABLE * HEIGHT_TABLE
#define NPATTERNS 						FILL_PATTERNS_AMOUNT
#define GET_TABLE_VALUE(table, x, y)	table[(y) * 2 * WIDTH_PTRN + (x) + WIDTH_PTRN]


typedef int Pattern[WIDTH_PTRN * HEIGHT_PTRN / 16];

struct PatternsArray {
	Pattern ptrns[FILL_PATTERNS_AMOUNT];
	Pattern linePtrns[LINE_PATTERNS_AMOUNT];
	Pattern pointPtrns[POINT_PATTERNS_AMOUNT];
	int table_dydx[SIZE_TABLE];
	int lineTable[SIZE_TABLE];
};

void fillPtrnsInit(unsigned char* dst, int* table_dydx, unsigned char color);
void linePtrnsInit(unsigned char* dst, int* table, unsigned char  color);
void pointPtrnsInit(unsigned char* dst, unsigned char color);

void create_tabl_dydx(unsigned char* srcPatterns, int* dydx, int width, int height);

void hostCreatePatterns(PatternsArray* patterns);
