#include "VShell.h"
#include "hal.h"
#include "hal_host.h"
#include "stdio.h"
#include "nmpp.h"
#include "demo3d_host.h"
#include "demo3d_nm1.h"
#include "hostprofiler.h"
#include "nmglvs_host.h"
#include <thread>
#include <iostream>
#include <fstream>


using namespace std;

int currentImage[WIDTH_IMAGE * HEIGHT_IMAGE];
void trace_func(int trace_addr, int trace_size, int delay) {
	int size32 = trace_size * 2;
	int* trace = new int[size32];
	halSleep(5000);
	while (true) {
		halSleep(delay);
		int point = 0;
		while (point < size32) {
			int localSize = MIN(size32 - point, 1024);
			halReadMemBlock(trace + point, trace_addr + point, localSize);
			point += 1024;
		}
		cout << "write file...";
		ofstream fout;
		fout.open("trace.bin", ios::binary | ios_base::out);
		if (fout.is_open()) {
			for (int i = 0; i < size32; i++) {
				fout.write((char*)(trace + i), sizeof(int));
			}
			fout.close();
		}
		cout << "OK!!\n";
	}
}

int main()
{
	if (nmglvsHostInit() != 0) {
		return 1;
	}
	int trace_addr = halSync(0, 1);
	int trace_size = halSync(0, 1);
	
	
	//----------------init-VShell--------------------------------------------
	if (!VS_Init())
		return 0;
	VS_CreateImage("Source Image", 1, WIDTH_IMAGE, HEIGHT_IMAGE, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	VS_CreateCheckBox("profiler0", 1, false);
	VS_CreateCheckBox("profiler1", 2, false);
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

	delete vertices;
	delete normal;
	//halFree(vertices);
	//halFree(normal);
	int flag = -1;
	int counter = 0;

	//std::thread thr(trace_func, trace_addr, trace_size, 10);
	//thr.detach();
	while(VS_Run())	{
#ifdef EMULATION
		halSleep(100);
#endif //EMULATION
		nmglvsHostReadImage(currentImage);
		VS_SetData(1, currentImage);
		counter++;
		if (counter>=256 && flag) {
			halProfilerPrint2xml("main0d.map", 0, "../perf0_1.xml");
			return 0;
			//halProfilerPrint2xml("main1.map", 1, "../perf1.xml");
			//return 0;
			flag = 0;
		}
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
