#include "VShell.h"
#include "hal.h"
#include "hal_host.h"
#include "stdio.h"
#include "nmpp.h"
#include "demo3d_host.h"
#include "hostprofiler.h"
#include "nmglvs_host.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <windows.h>

#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include <mutex>

using namespace std;

const int step = 8;
int max = 0;
mutex mut;
vector<int> reps;

int currentImage[WIDTH_IMAGE * HEIGHT_IMAGE];

void update_count_rep(const map<string, map<string, string>> &dump_dict, const vector<string> &trace, vector<int> &reps);
void get_dump_dict(string file_name, map<string, map<string, string>> &dump_map);

bool myRun() {
	mut.lock();
	bool result = VS_Run();
	mut.unlock();
	return result;
}

void trace_func(int trace_addr, int trace_size, map<string, map<string,string>> &dump_map, int delay) {

	for (int i = 0; i < 32; i++) {
		reps.push_back(0);
	}
	int size32 = trace_size * 2;
	int* trace = new int[size32];
	//halSleep(5000);
	while (true) {
		
		halSleep(delay);
		int point = 0;
		vector<string> trace_vec;
		mut.lock();
		while (point < size32) {
			int localSize = MIN(size32 - point, 1024);
			halReadMemBlock(trace + point, trace_addr + point, localSize);
			point += 1024;
		}
		mut.unlock();
		for (int i = 0; i < trace_size; i++) {
			std::stringstream stream;
			stream << std::hex << trace[2*i];
			trace_vec.push_back(stream.str());
		}
		update_count_rep(dump_map, trace_vec, reps);
		for (int i = 0; i < 32; i++) {
			max = (reps[i] > max) ? reps[i] : max;
		}
	}
}


int main()
{
	if (nmglvsHostInit() != 0) {
		return 1;
	}
	int trace_addr = halSync(0, 1);
	int trace_size = halSync(0, 1);

	clock_t t0, t1;
	t0 = clock();
	//map<string, map<string, string>> dump_map;
	//get_dump_dict("main1d.dasm", dump_map);
	t1 = clock();
	//cout << "time get_dump_dict: " << t1 - t0 << endl;

	//----------------init-VShell--------------------------------------------
	if (!VS_Init())
		return 0;

	VS_CreateImage("Source Image", 1, WIDTH_IMAGE, HEIGHT_IMAGE, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
	VS_CreateImage("Source Image", 2, step * 32, step * 32, VS_RGB32, 0);	// Create window for 8-bit source grayscale image
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

	//std::thread thr(trace_func, trace_addr, trace_size, dump_map, 10);
	//thr.detach();
	while(myRun())	{
#ifdef EMULATION
		halSleep(100);
#endif //EMULATION
		nmglvsHostReadImage(currentImage);
		VS_SetData(1, currentImage);
		if (max > 0) {
			for (int i = 0; i < 32; i++) {
				VS_RectangleF(2, step * i, 32.0 * step * (1 - (float)reps[i] / (1.2 * max)), step * (i + 1), 32.0 * step, VS_WHITE, VS_BLACK);
			}
		}
		counter++;
		if (counter>=256 && flag) {
			//halProfilerPrint2xml("main0d.map", 0, "../perf0.xml");
			//return 0;
			//halProfilerPrint2xml("main1d.map", 1, "../perf1.xml");
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
