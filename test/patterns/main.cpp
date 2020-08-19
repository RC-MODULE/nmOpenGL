#include "VShell.h"
#include "windows.h"
#include "pattern.h"
#include "nmpp.h"
#include "stdio.h"
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
	VS_CreateImage("oldfill", 4, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("lineTable", 5, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	//VS_OpRunForward();

	unsigned char* fillPtrns = new unsigned char[FILL_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* fillPtrnsOld = new unsigned char[FILL_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* linePtrns = new unsigned char[LINE_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* pointPtrns = new unsigned char[POINT_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	int *lineTable = new int[2 * WIDTH_PTRN * HEIGHT_PTRN + 2 * WIDTH_PTRN];
	int *fillTable = new int[2 * WIDTH_PTRN * HEIGHT_PTRN + 2 * WIDTH_PTRN];

	nmppsSet_8s((nm8s*)fillPtrnsOld, 0, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	fillPattern((nm8s*)fillPtrnsOld, WIDTH_PTRN, HEIGHT_PTRN);
	fillPtrnsInit(fillPtrns, fillTable, -1);
	linePtrnsInit(linePtrns, lineTable, -1);
	pointPtrnsInit(pointPtrns, -1);
	int size = sizeof(PatternsArray);

	unsigned int crc1 = 0;
	unsigned int crc2 = 0;
	nmppsCrcAcc_8u(fillPtrns, FILL_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN, &crc1);
	nmppsCrcAcc_8u(fillPtrnsOld, FILL_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN, &crc2);
	//printf("new=0x%x, old=0x%x\n", crc1, crc2);
	//printPtrnsToTxt(fillPtrns, "newpatterns.txt");
	//printPtrnsToTxt(fillPtrnsOld, "oldpatterns.txt");
	//createLineTable(linePtrnsOne, lineTableGte, lineTableLt);

	int fillCnt = 0;
	int lineCnt = 0;
	int pointCnt = 0;
	int dy = 15;
	int dx = 15;
	while(VS_Run())	{
		VS_SetData(1, fillPtrns + fillCnt * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(2, linePtrns + lineCnt * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(3, pointPtrns + pointCnt * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(4, fillPtrnsOld + fillCnt * WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(5, linePtrns + lineTable[dy * 2 * WIDTH_PTRN + dx + WIDTH_PTRN] * WIDTH_PTRN * HEIGHT_PTRN);
		//VS_SetData(5, linePtrns + 64 * WIDTH_PTRN * HEIGHT_PTRN);
		dx++;
		fillCnt++;
		lineCnt++;
		pointCnt++;
		Sleep(10);
		if (fillCnt >= FILL_PATTERNS_AMOUNT) {
			fillCnt = 0;
		}
		if (lineCnt >= LINE_PATTERNS_AMOUNT) {
			lineCnt = 0;
		}
		if (pointCnt >= POINT_PATTERNS_AMOUNT) {
			pointCnt = 0;
		}
		VS_Draw(VS_DRAW_ALL);
	}
	delete fillPtrns;
	delete fillPtrnsOld;
	delete linePtrns;
	delete pointPtrns;
	delete lineTable;
	delete fillTable;
	return 0;
};
