#include "VShell.h"
#include "pattern.h"
#include "nmpp.h"
#include "stdio.h"
#include "windows.h"
#include <iostream>
#include <fstream>

using namespace std;

#define SCALE 20

#define CENTER_PIXEL_X 0.5
#define CENTER_PIXEL_Y 0.5

void printPtrnsToTxt(unsigned char * ptrns, char* path) {
	ofstream ptrnstream;          // поток для записи
	ptrnstream.open(path); // окрываем файл для записи
	if (ptrnstream.is_open())
	{
		for (int i = 0; i < FILL_PATTERNS_AMOUNT; i++) {
			ptrnstream << i << endl;
			unsigned char* addr = ptrns + i * WIDTH_PTRN * HEIGHT_PTRN;
			for (int y = 0; y < HEIGHT_PTRN; y++) {
				for (int x = 0; x < HEIGHT_PTRN; x++) {
					if (addr[y * HEIGHT_PTRN + x]) {
						ptrnstream << 1;
					}
					else {
						ptrnstream << 0;
					}
				}
				ptrnstream << endl;
			}
			ptrnstream << endl;
		}
	}
	ptrnstream.close();
}


void fillPattern(nm8s* pDstSource, int width, int height);

int main()
{
	if (!VS_Init())
		return 0;
	VS_CreateImage("fill", 1, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("antifill", 2, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("line", 3, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("point", 4, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("lineTableTest", 5, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("fillTableTest", 6, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	
	
	//VS_OpRunForward();

	unsigned char* fillPtrns = new unsigned char[FILL_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* linePtrns = new unsigned char[LINE_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* pointPtrns = new unsigned char[POINT_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* sumFill = new unsigned char[WIDTH_PTRN * HEIGHT_PTRN];
	int *lineTable = new int[SIZE_TABLE];
	int *fillTable = new int[SIZE_TABLE];
	fillPtrnsInit(fillPtrns, fillTable, -1);
	linePtrnsInit(linePtrns, lineTable, -1);
	pointPtrnsInit(pointPtrns, -1);
	int size = sizeof(PatternsArray);

	//printPtrnsToTxt(fillPtrns, "patterns.txt");
	ofstream ptrnstream;          // поток для записи
	ptrnstream.open("table.txt"); // окрываем файл для записи
	if (ptrnstream.is_open()) {
		for (int y = 0; y <= HEIGHT_PTRN; y++) {
			for (int x = -WIDTH_PTRN; x < WIDTH_PTRN; x++) {
				ptrnstream.width(5);
				ptrnstream << fillTable[y * 2 * WIDTH_PTRN + x + WIDTH_PTRN];
			}
			ptrnstream << endl;
		}
	}
	ptrnstream.close();
	int yTable = 0;
	int xTable = 0;
	int fillCnt = 0;
	int antifillCnt = FILL_PATTERNS_AMOUNT / 2;
	int lineCnt = 0;
	int pointCnt = 0;
	int check1[][2] = { {-31,0}, {-15,3}, {-15,15}, {-3,15}, { 0,0 } };
	int check2[][2] = { { 31,0 },{ 15,3 },{ 15,15 },{ 3,15 },{ 0,0 } };
	int i = 0;
	while(VS_Run())	{
		int sizePtrn = WIDTH_PTRN * HEIGHT_PTRN;
		//int number0 = GET_TABLE_VALUE(fillTable, 30, 4);
		//int number1 = GET_TABLE_VALUE(fillTable, 30, 4) + FILL_PATTERNS_AMOUNT / 2;
		int number0 = fillCnt;
		int number1 = fillCnt + FILL_PATTERNS_AMOUNT / 2;
		
		VS_SetData(1, fillPtrns + number0 * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(2, fillPtrns + number1 * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(3, linePtrns + lineCnt * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(4, pointPtrns + pointCnt * WIDTH_PTRN * HEIGHT_PTRN);
		//VS_SetData(5, fillPtrns + (GET_TABLE_VALUE(fillTable, check1[i][0], check1[i][1]) + 31) * WIDTH_PTRN * HEIGHT_PTRN);
		//VS_SetData(6, fillPtrns + GET_TABLE_VALUE(fillTable, check2[i][0], check2[i][1]) * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(5, fillPtrns + GET_TABLE_VALUE(fillTable, 31, 4) * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(6, fillPtrns + (3840) * WIDTH_PTRN * HEIGHT_PTRN);
		
		//int h = GET_TABLE_VALUE(lineTable, values[i][0], values[i][1]);

		//VS_Text("fillNo = %d\r\n", fillCnt);
		//VS_Text("lineNo = %d\r\n", lineCnt);
		//VS_Text("pointNo = %d\r\n\r\n", pointCnt);

		xTable++;
		if (xTable == WIDTH_PTRN) {
			yTable++;
			xTable = 0;
			if (yTable == HEIGHT_PTRN) {
				yTable = 0;
			}
		}
		i++;
		fillCnt++;
		lineCnt++;
		pointCnt++;
		Sleep(10);
		i = (i == 9) ? 0 : i;
		if (fillCnt >= FILL_PATTERNS_AMOUNT / 2) {
			fillCnt = 0;
			antifillCnt = FILL_PATTERNS_AMOUNT / 2;
		}
		lineCnt = (lineCnt >= LINE_PATTERNS_AMOUNT) ? 0 : lineCnt;
		pointCnt = (pointCnt >= POINT_PATTERNS_AMOUNT) ? 0 : pointCnt;

		VS_Draw(VS_DRAW_ALL);
	}
	delete fillPtrns;
	delete linePtrns;
	delete pointPtrns;
	delete lineTable;
	delete fillTable;
	return 0;
};
