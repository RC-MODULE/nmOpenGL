#include "vshell.h"
#include "hal.h"
#include "hal_host.h"
#include "stdio.h"
#include "hostprofiler.h"
#include "framebuffer.h"

#include "demo3d_host.h"


int currentImage[WIDTH_IMAGE * HEIGHT_IMAGE];
int currentDepth[WIDTH_IMAGE * HEIGHT_IMAGE];

void readColorBackNM(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void readDepthNM(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
void readColorFrontNM(void *data, NMGL_FrameBuffer *fb, int x, int y, int width, int height);
bool frameBufferIsEmpty(NMGL_FrameBuffer *remoteAddr);
void frameBufferIncTail(NMGL_FrameBuffer *remoteAddr);


int main()
{
	//printf(PROGRAM + "\n");
	//printf(PROGRAM1 + "\n");
	// load programs to nm
	if (halOpen(PROGRAM, PROGRAM1, NULL))
	//if (halOpen(".", ".", NULL))
	{
		printf("Connection to mc12101 error!\n");
		return -1;
	}

	// check connection sync
	int handshake = halSync(0xC0DE0086, 0);
	if (handshake != 0xC0DE0000)
	{
		printf("Handshake with mc12101-nmc0 error!\n");
		return -1;
	}

	// get framebuffer address
	NMGL_FrameBuffer *remote = (NMGL_FrameBuffer *)halSyncAddr(0, 0);

	//----------------init-VShell--------------------------------------------
	if (!VS_Init())
		return 0;

	VS_CreateImage("Source Image", 1, WIDTH_IMAGE, HEIGHT_IMAGE, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	//VS_CreateImage("Source Depth", 2, WIDTH_IMAGE, HEIGHT_IMAGE, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	
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
		// wait finish image in nm
		while (frameBufferIsEmpty(remote));

		// read back color buffer
		readColorBackNM(currentImage, remote, 0, 0, WIDTH_IMAGE, HEIGHT_IMAGE);

		// set finish copy flaf
		frameBufferIncTail(remote);

		counter++;
		if (counter>=256 && flag) {
			//halProfilerPrint2xml("main0d.map", 0, "../perf0.xml");
			//halProfilerPrint2xml("main1d.map", 1, "../perf1.xml");
			//return 0;
			flag = 0;
		}

		VS_SetData(1, currentImage);
		VS_Draw(VS_DRAW_ALL);
	}
	unsigned int result[2];
	//nmglvsExit(result);
	return 0;
};
