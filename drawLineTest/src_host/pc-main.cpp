#include "VShell.h"
#include "nmglvs_host.h"

#include "demo3d_host.h"

#include "hal.h"
#include "hal_host.h"
#include "stdio.h"



using namespace std;

int currentImage[WIDTH_IMAGE * HEIGHT_IMAGE];


int main()
{
	if (nmglvsHostInit() != 0) {
		return 1;
	}

	//----------------init-VShell--------------------------------------------
	if (!VS_Init())
		return 0;
	VS_CreateImage("Source Image", 1, WIDTH_IMAGE, HEIGHT_IMAGE, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("Pattern", 2, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);	// Create window for 8-bit source grayscale image
	//VS_OpRunForward();
	int ok;

	Models models;
	char* filePath = models.nm;
	FILE* fmodel = fopen(filePath, "r");
	int amount = get_amm_poligone(fmodel);
	fmodel = fopen(filePath, "r");
	float* vertices = new float [12 * 2000];
	float* vertices2 = new float[24 * 2000];
	float* normal = new float[12 * 2000];
	createArrayVec4(fmodel, vertices, normal, 1);
	cnvTrianglesToLines(vertices, vertices2, amount);
	int verticesNM = halSync(1, 0);
	ok = halWriteMemBlock(vertices2, verticesNM, amount * 24, 0);
	ok = halSync(3 * amount, 0);

	PatternsArray* patterns = (PatternsArray*)nmppsMalloc_32s(sizeof32(PatternsArray));
	hostCreatePatterns(patterns);
	int counter = 0;
	signed char ptrn8[WIDTH_PTRN * HEIGHT_PTRN];
	char ptrn4[WIDTH_PTRN * HEIGHT_PTRN / 2];

	while(VS_Run())	{
		nmglvsHostReadImage(currentImage);

		VS_SetData(1, currentImage);
		nmppsConvert_2s4s(patterns->linePtrns[counter++], ptrn4, WIDTH_PTRN * HEIGHT_PTRN);
		if (counter == LINE_PATTERNS_AMOUNT) {
			counter = 0;
		}
		nmppsConvert_4s8s(ptrn4, ptrn8, WIDTH_PTRN * HEIGHT_PTRN);
		VS_SetData(2, ptrn8);
		VS_Draw(VS_DRAW_ALL);
	}

	unsigned int result[2];
	nmglvsExit(result);
	return 0;
};
