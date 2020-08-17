#include "VShell.h"
#include "windows.h"
#include "pattern.h"

using namespace std;



int main()
{
	if (!VS_Init())
		return 0;
	VS_CreateImage("Source Image", 1, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	//VS_OpRunForward();

	unsigned char* fillPtrns = new unsigned char[FILL_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* linePtrns = new unsigned char[LINE_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];
	unsigned char* pointPtrns = new unsigned char[POINT_PATTERNS_AMOUNT * WIDTH_PTRN * HEIGHT_PTRN];

	fillPtrnsInit(fillPtrns, -1);
	linePtrnsInit(linePtrns, -1);
	pointPtrnsInit(pointPtrns, -1);

	int counter = 0;
	while(VS_Run())	{
		VS_SetData(1, linePtrns + counter * WIDTH_PTRN * HEIGHT_PTRN);
		//counter += OFFSETS;
		counter++;
		Sleep(10);
		if (counter >= LINE_PATTERNS_AMOUNT) {
			counter = 0;
		}
		VS_Draw(VS_DRAW_ALL);
	}
	return 0;
};
