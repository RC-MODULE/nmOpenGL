//------------------------------------------------------------------------
//  Demostration for MAX 2017 
//
//  Author: Alexander Bolornikov
//
//  Copyright (c) 2017 RC Module Inc.
//------------------------------------------------------------------------

#include "VShell.h"
#include "hal.h"
#include "hal_host.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "nmpp.h"
#include "demo3d_host.h"
#include "demo3d_nm1.h"
#include "ringbuffer_host.h"
#include <thread>
#include "hostprofiler.h"
#include "nmglvs_host.h"

using namespace std;

int currentImage[WIDTH_IMAGE * HEIGHT_IMAGE];

extern int synchro_nm;

void fillPattern(nm8s* pDstSource, int width, int height);
void create_tabl_dydx(nm8s* srcPatterns, int* dydx, int width, int height);

int main()
{
	if (nmglvsHostInit() != 0) {
		return 1;
	}

	//----------------init-VShell--------------------------------------------
	if (!VS_Init())
		return 0;
	VS_CreateImage("Source Image", 1, WIDTH_IMAGE, HEIGHT_IMAGE, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("Pattern", 2, WIDTH_PTRN, HEIGHT_PTRN, VS_RGB8, 0);
	VS_CreateImage("Table", 3, 2 * WIDTH_PTRN, HEIGHT_PTRN, VS_RGB32, 0);
	VS_OpRunForward();

	float* vertices = new float[4000 * 12];
	float* normal = new float[4000 * 9];
	int ok;

	int sizePolygons = sizeof32(Polygons);

	Models models;
	char* filePath = models.nm;
	
	FILE* fmodel = fopen(filePath, "r");
	int amount = get_amm_poligone(fmodel);
	fmodel = fopen(filePath, "r");
	createArrayVec4(fmodel, vertices, normal, 1);
	ok = halSync(amount, 0);
	int verticesNM = halSync(1, 0);
	ok = halWriteMemBlock(vertices, verticesNM, amount * 12, 0);
	int normalNM = halSync(3, 0);
	ok = halWriteMemBlock(normal, normalNM, amount * 9, 0);

	filePath = models.two_sphere;
	fmodel = fopen(filePath, "r");
	amount = get_amm_poligone(fmodel);
	fmodel = fopen(filePath, "r");
	createArrayVec4(fmodel, vertices, normal, 0.5);
	halSync(amount, 0);
	verticesNM = halSync(1, 0);
	ok = halWriteMemBlock(vertices, verticesNM, amount * 12, 0);
	normalNM = halSync(3, 0);
	ok = halWriteMemBlock(normal, normalNM, amount * 9, 0);
	ok = halSync(4, 0);

	delete vertices;
	delete normal;
	int flag = -1;

	int dydx[2 * WIDTH_PTRN * (HEIGHT_PTRN + 2)];
	nm8s* temp_8s = nmppsMalloc_8s(NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	nmppsSet_8s((nm8s*)temp_8s, 0, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	fillPattern(temp_8s, WIDTH_PTRN, HEIGHT_PTRN);
	create_tabl_dydx(temp_8s, dydx, WIDTH_PTRN, HEIGHT_PTRN);
	nmppsMulC_8s(temp_8s, -1, temp_8s, NPATTERNS*WIDTH_PTRN*HEIGHT_PTRN);
	int counter = 0;

	while(VS_Run())	{
		nmglvsHostReadImage(currentImage);

		VS_SetData(2, nmppsAddr_8s((nm8s*)temp_8s, counter++ * WIDTH_PTRN * HEIGHT_PTRN));
		if (counter == NPATTERNS) {
			halSleep(5000);
			counter = 0;
		}
		VS_SetData(3, dydx);
		VS_SetData(1, currentImage);
		int counter[2] = { 0 ,0};
		ok = halReadMemBlock(&counter, synchro_nm, 2, 1);
		if (counter[0]>=256 && flag) {
			//halProfilerPrint2xml(".main0.map", 0, "../perf0.xml");
			//halProfilerPrint2xml(".main1.map", 1, "../perf1.xml");
			//halProfilerPrint2tbl(".main0.map", 0);
			flag = 0;
		}
		//drawMeshSegments(currentImage);
		/*VS_Line(1, 0, 128, 768, 128, 0xFF);
		VS_Line(1, 0, 256, 768, 256, 0xFF);
		VS_Line(1, 0, 384, 768, 384, 0xFF);
		VS_Line(1, 0, 512, 768, 512, 0xFF);
		VS_Line(1, 0, 640, 768, 640, 0xFF);

		VS_Line(1, 128, 0, 128, 768, 0xFF);
		VS_Line(1, 256, 0, 256, 768, 0xFF);
		VS_Line(1, 384, 0, 384, 768, 0xFF);
		VS_Line(1, 512, 0, 512, 768, 0xFF);
		VS_Line(1, 640, 0, 640, 768, 0xFF);*/


		VS_Draw(VS_DRAW_ALL);
		//VS_Text("");
	}
	nmppsFree(temp_8s);
	unsigned int result[2];
	nmglvsExit(result);
	return 0;
};
