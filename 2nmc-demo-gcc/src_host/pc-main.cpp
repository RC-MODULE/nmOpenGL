#include "VShell.h"
#include "hal.h"
#include "hal_host.h"
#include "stdio.h"
#include "demo3d_common.h"
#include "demo3d_host.h"
#include "hostprofiler.h"
#include "nmglvs_host.h"
#include <fstream>

#include "imagebuffer_host.h"


int main()
{
	
	/*if (nmglvsHostInit() != 0) {
		return 1;
	}*/

	if (halOpen(PROGRAM, PROGRAM1, NULL)) {
		printf("Connection to mc12101 error!\n");
		return -1;
	}

	int handshake = halSync(0xC0DE0086, 0);
	if (handshake != 0xC0DE0000) {
		printf("Handshake with mc12101-nmc0 error!\n");
		return -1;
	}

	int framebufferAddr = halSyncAddr(0, 0);
	NMGL_FramebufferHost framebuffer(framebufferAddr, 0);

	//----------------init-VShell--------------------------------------------
	if (!VS_Init())
		return 0;

	int width = framebuffer.getWidth();
	int height = framebuffer.getHeight();
	VS_CreateImage("Source Image", 1, width, height, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	int *currentImage = new int[width * height];
	VS_OpRunForward();

	float* vertices = new float[4000 * 12];
	float* normal = new float[4000 * 9];
	int ok;

	Models models;
	char* filePath = models.nm;
	
	FILE* fmodel = fopen(filePath, "r");
	int amount = get_amm_poligone(fmodel);
	fclose(fmodel);
	fmodel = fopen(filePath, "r");
	createArrayVec4(fmodel, vertices, normal, 1);
	fclose(fmodel);
	ok = halSync(amount, 0);
	int verticesNM = halSyncAddr(0, 0);
	ok = halWriteMemBlock(vertices, verticesNM, amount * 12, 0);
	int normalNM = halSyncAddr(0, 0);
	ok = halWriteMemBlock(normal, normalNM, amount * 9, 0);

	filePath = models.two_sphere;
	fmodel = fopen(filePath, "r");
	amount = get_amm_poligone(fmodel);
	fclose(fmodel);
	fmodel = fopen(filePath, "r");
	createArrayVec4(fmodel, vertices, normal, 0.5);
	fclose(fmodel);
	halSync(amount, 0);
	verticesNM = halSyncAddr(0, 0);
	ok = halWriteMemBlock(vertices, verticesNM, amount * 12, 0);
	normalNM = halSyncAddr(0, 0);
	ok = halWriteMemBlock(normal, normalNM, amount * 9, 0);
	ok = halSync(4, 0);

	delete[] vertices;
	delete[] normal;
	int flag = -1;
	int counter = 0;

	while (VS_Run()) {
#ifdef EMULATION
		halSleep(100);
#endif //EMULATION
		framebuffer.readImage(currentImage);
		//nmglvsHostReadImage(currentImage);
		VS_SetData(1, currentImage);
		
		counter++;
		if (counter>=256 && flag) {
			//halProfilerPrint2xml("main0d.map", 0, "../perf0.xml");
			//return 0;
			//halProfilerPrint2xml("main1d.map", 1, "../perf1.xml");
			//return 0;
			flag = 0;
		}

		VS_Draw(VS_DRAW_ALL);
	}
	unsigned int result[2];
	delete[] currentImage;
	nmglvsExit(result);
	return 0;
};
