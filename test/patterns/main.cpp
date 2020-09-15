#include "VShell.h"
#include "pattern.h"
#include "nmpp.h"
#include "stdio.h"
#include "windows.h"
#include <iostream>
#include <fstream>

using namespace std;

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
	VS_CreateImage("line", 2, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("point", 3, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("lineTableTest", 4, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("fillTableTest", 5, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	//VS_OpRunForward();

	unsigned char* fillPtrns = new unsigned char[FILL_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* linePtrns = new unsigned char[LINE_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* pointPtrns = new unsigned char[POINT_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	int *lineTable = new int[SIZE_TABLE];
	int *fillTable = new int[SIZE_TABLE];
	fillPtrnsInit(fillPtrns, fillTable, -1);
	linePtrnsInit(linePtrns, lineTable, -1);
	pointPtrnsInit(pointPtrns, -1);
	int size = sizeof(PatternsArray);

	//printPtrnsToTxt(fillPtrns, "patterns.txt");

	int fillCnt = 0;
	int lineCnt = 0;
	int pointCnt = 0;
	int values[][2] = { {-30,0}, {-15,3}, {-15,15}, {-3,15}, { 0,30 }, { 3,15 }, { 15,15 }, { 15,3 }, { 30,0 } };
	int d[][2] = { {0,9}, {-5, 4}, {5,5} };
	int i = 0;
	while(VS_Run())	{
		VS_SetData(1, fillPtrns + fillCnt * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(2, linePtrns + lineCnt * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(3, pointPtrns + pointCnt * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(4, linePtrns + GET_TABLE_VALUE(lineTable, values[i][0], values[i][1]) * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(5, fillPtrns + GET_TABLE_VALUE(fillTable, values[i][0], values[i][1]) * WIDTH_PTRN * HEIGHT_PTRN);
		int h = GET_TABLE_VALUE(lineTable, values[i][0], values[i][1]);
		i++;
		fillCnt++;
		lineCnt++;
		pointCnt++;
		Sleep(10);
		i = (i == 9) ? 0 : i;
		fillCnt = (fillCnt >= FILL_PATTERNS_AMOUNT) ? 0 : fillCnt;
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
