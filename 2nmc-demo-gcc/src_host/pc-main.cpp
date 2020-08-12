#include "VShell.h"
#include "hal.h"
#include "hal_host.h"
#include "stdio.h"
#include "nmpp.h"
#include "demo3d_host.h"
#include "demo3d_nm1.h"
#include "hostprofiler.h"
#include "nmglvs_host.h"

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
	//halFree(vertices);
	//halFree(normal);
	int flag = -1;
	int counter = 0;

	while(VS_Run())	{
#ifdef EMULATION
		halSleep(100);
#endif //EMULATION
		nmglvsHostReadImage(currentImage);
		VS_SetData(1, currentImage);
		counter++;
#if defined(PROFILER0) || defined(PROFILER1)
		if (counter>=256 && flag) {
#ifdef PROFILER0
			halProfilerPrint2xml("main0.map", 0, "../perf0.xml");
			//return 0;
#endif // PROFILER0
#ifdef PROFILER1
			halProfilerPrint2xml("main1.map", 1, "../perf1.xml");
#endif // PROFILER1
			flag = 0;
		}
#endif
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
	unsigned int result[2];
	nmglvsExit(result);
	return 0;
};
