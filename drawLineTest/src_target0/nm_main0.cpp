#include "nmgl.h"
#include "hal.h"
#include "nmglvs_nmc0.h"
#include "hal_target.h"

SECTION(".data_shared0") float lines[24 * 2] = {
	110, 90, 
	90, 110,
	
	90, 90,
	110, 110,

	90, 110,
	110, 110,

	110, 90,
	110, 110,

	90, 90,
	70, 95,

	110, 90,
	130, 95,

	90, 90,
	85, 110,

	110, 90,
	115, 110,
	
};

int main()
{
	nmglvsNm0Init();
#ifndef __OPEN_GL__
	setHeap(10);
	float* vertices = (float*)halMalloc32(24 * 2000);
	halHostSync((int)vertices);
	int amount = halHostSync(0x600DB00F);
#else
	Models models;
	char* filePath = models.nm;
	FILE* fmodel = fopen(filePath, "r");
	int amount = get_amm_poligone(fmodel);
	fmodel = fopen(filePath, "r");
	float* vert = new float [12 * 2000];
	float* vertices = new float[24 * 2000];
	float* normal = new float[12 * 2000];
	createArrayVec4(fmodel, vert, normal, 1);
	cnvTrianglesToLines(vert, vertices, amount);
	amount *= 3;
#endif

	nmglClearColor(0.5, 0.5, 0.5f, 1.0f);

	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(-768 /2, 768 / 2, -768 / 2, 768 / 2, 0, 100);
	
	//nmglVertexPointer(2, NMGL_FLOAT, 0, lines);
	nmglVertexPointer(4, NMGL_FLOAT, 0, vertices);
	
	nmglEnableClientState(NMGL_VERTEX_ARRAY);
	nmglMatrixMode(NMGL_MODELVIEW);
	while(nmglvsNm0Run()){
		nmglClear(NMGL_COLOR_BUFFER_BIT);

		nmglRotatef(0.3, 0.707, 0.707, 0);
		//nmglDrawArrays(NMGL_LINES, 0, 24);
		nmglDrawArrays(NMGL_LINES, 0, 2 * amount);

		nmglvsSwapBuffer();
	}
	nmglDisableClientState(NMGL_VERTEX_ARRAY);
	nmglvsExit_mc12101();
	return 0x600D600D;
}