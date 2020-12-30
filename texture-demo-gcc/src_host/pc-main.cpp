#include "VShell.h"
#include "nmglvs_host.h"
#include "demo3d_common.h"
#include "hostprofiler.h"



using namespace std;

int currentImage[WIDTH_IMAGE * HEIGHT_IMAGE];

int main()
{
	if (nmglvsHostInit() != 0) {
		return 1;
	}
	
	if (!VS_Init())
		return 0;
	VS_CreateImage("Source Image", 1, WIDTH_IMAGE, HEIGHT_IMAGE, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	VS_OpRunForward();


	while(VS_Run())	{
		nmglvsHostReadImage(currentImage);
		//halProfilerPrint2xml("main0d.map", 0, "../perf0.xml");
		//return 0;

		VS_SetData(1, currentImage);
		VS_Draw(VS_DRAW_ALL);
	}

	unsigned int result[2];
	nmglvsExit(result);
	return 0;
};
