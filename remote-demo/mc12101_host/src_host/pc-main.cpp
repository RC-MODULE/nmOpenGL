#include "VShell.h"
#include "hal.h"
#include "hal_host.h"
#include "stdio.h"
#include "demo3d_common.h"
#include "demo3d_host.h"
#include "hostprofiler.h"
#include "nmglvs_host.h"
#include <fstream>


int main()
{
	if (nmglvsHostInit() != 0) {
		return 1;
	}

	//----------------init-VShell--------------------------------------------
	if (!VS_Init())
		return 0;

	int width = nmglvsGetWidth();
	int height = nmglvsGetHeight();
	VS_CreateImage("Source Image", 1, width, height, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	int *currentImage = new int[width * height];
	VS_OpRunForward();

	int flag = -1;
	int counter = 0;

	while (VS_Run()) {
#ifdef EMULATION
		halSleep(100);
#endif //EMULATION
		nmglvsHostReadImage(currentImage);
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
