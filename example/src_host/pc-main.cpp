#include "VShell.h"
#include "nmglvs_host.h"
#include "demo3d_common.h"



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

		VS_SetData(1, currentImage);

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
	}

	unsigned int result[2];
	nmglvsExit(result);
	return 0;
};
