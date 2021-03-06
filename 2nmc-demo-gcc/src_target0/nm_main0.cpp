#include "hal_target.h"
#include "hal.h"
#include <time.h>
#include "stdio.h"
#include "nmpp.h"
#include "nmgl.h"
#include "nmglvs_nmc0.h"
#include "nmprofiler.h"

SECTION(".text_shared0") int main()
{
	halSleep(3000);
	//nmprofiler_init();
	//nmprofiler_disable();

#if defined(__GNUC__) && defined(PROFILER0)
	nmprofiler_init();
	nmprofiler_enable();
#endif // __GNUC__

	nmglvsNm0Init();

	
#ifdef __OPEN_GL__

	setHeap(10);
	float* vertices_DDR = new float[2000 * 12];
	float* normal_DDR = new float[2000 * 9];
	float* vertices_DDR2 = new float[2000 * 12];
	float* normal_DDR2 = new float[2000 * 9];
	int ok;

	Models models;
	char* filePath = models.nm;

	FILE* fmodel = fopen(filePath, "r");
	if (fmodel == 0) {
		printf("File not opened\n");
		return 1;
	}
	int amount = get_amm_poligone(fmodel);
	fmodel = fopen(filePath, "r");
	createArrayVec4(fmodel, vertices_DDR, normal_DDR, 1);
	int amountPolygons = amount;

	filePath = models.two_sphere;
	fmodel = fopen(filePath, "r");
	amount = get_amm_poligone(fmodel);
	fclose(fmodel);
	fmodel = fopen(filePath, "r");
	createArrayVec4(fmodel, vertices_DDR2, normal_DDR2, 0.5);
	fclose(fmodel);

	int amountPolygons2 = amount;
#else

	setHeap(10);
	float* vertices_DDR = (float*)halMalloc32(2000 * 12);
	float* normal_DDR = (float*)halMalloc32(2000 * 9);
	float* vertices_DDR2 = (float*)halMalloc32(2000 * 12);
	float* normal_DDR2 = (float*)halMalloc32(2000 * 9);
	int ok;
	int amountPolygons = halHostSync(0);

	//������ ������ ��������� (�� 4 ���������� �� �������)
	//sync1
	halHostSyncAddr(vertices_DDR);

	//sync2
	halHostSyncAddr(normal_DDR);

	int amountPolygons2 = halHostSync(0);
	halHostSyncAddr(vertices_DDR2);

	//sync2
	halHostSyncAddr(normal_DDR2);
	ok = halHostSync((int)0x600D600D);
#endif
	nmglClearColor(0, 0, 0.4f, 0.0f);

	nmglEnable(NMGL_DEPTH_TEST);
	nmglEnable(NMGL_CULL_FACE);

	nmglMatrixMode(NMGL_MODELVIEW);
	nmglLoadIdentity();
	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(-384, 384, -384, 384, -384, 384);
	//nmglOrthof(-384, 384, -384, 384, -100, 100);
	//nmglFrustumf(-384, 384, -384, 384, 0, 100);
	nmglMatrixMode(NMGL_MODELVIEW);

	nmglViewport(0, 0, 768, 768);

	nmglEnable(NMGL_LIGHTING);
	nmglEnable(NMGL_LIGHT0);
	float lightVector[4] = { 0, -0.707, 0.707, 0 };
	nmglLightfv(NMGL_LIGHT0, NMGL_POSITION, lightVector);
	float lightDiffuse[4] = { 1, 1, 1, 1 }; 
	nmglLightfv(NMGL_LIGHT0, NMGL_DIFFUSE, lightDiffuse);
	float materialSpec[4] = { 0,0,0,1 };
	float materialDiffuse[4] = { 0,0,0,1 };
	nmglMaterialf(NMGL_FRONT_AND_BACK, NMGL_SHININESS, 15);
	//float angle = 1.72 * 38;
	float angle = 0;
	NMGLenum error;
	unsigned time;

	/*nmglEnable(NMGL_LIGHT1);
	float lightVector2[4] = { 0, 0.707, 0.707, 0 };
	nmglLightfv(NMGL_LIGHT1, NMGL_POSITION, lightVector);
	float lightDiffuse2[4] = { 1, 1, 0, 1 };
	nmglLightfv(NMGL_LIGHT1, NMGL_DIFFUSE, lightDiffuse);*/
	int counter = 0;
	while(nmglvsNm0Run()){
		nmglEnableClientState(NMGL_VERTEX_ARRAY);
		nmglEnableClientState(NMGL_NORMAL_ARRAY);

		nmglClear(NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT);

		nmglVertexPointer(4, NMGL_FLOAT, 0, vertices_DDR);
		nmglNormalPointer(NMGL_FLOAT, 0, normal_DDR);
		nmglLoadIdentity();
		//nmglScalef(0.9f, 0.9f, 0.9f);
		nmglRotatef(angle, 0.707f, 0.707f, 0);
		materialDiffuse[0] = 1;
		materialDiffuse[2] = 0;
		nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_DIFFUSE, materialDiffuse);
		materialSpec[0] = 0.6;
		materialSpec[1] = 0.6;
		materialSpec[2] = 0.6;
		nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_SPECULAR, materialSpec);
		PROFILER_SIZE(amountPolygons);
		nmglDrawArrays(NMGL_TRIANGLES, 0, 3 * amountPolygons);

		nmglVertexPointer(4, NMGL_FLOAT, 0, vertices_DDR2);
		nmglNormalPointer(NMGL_FLOAT, 0, normal_DDR2);
		nmglLoadIdentity();
		//nmglScalef(0.9f, 0.9f, 0.9f);
		materialDiffuse[0] = 0.4;
		materialDiffuse[1] = 1;
		materialDiffuse[2] = 1;
		materialSpec[0] = 0;
		materialSpec[1] = 0;
		materialSpec[2] = 0;
		nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_DIFFUSE, materialDiffuse);
		nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_SPECULAR, materialSpec);
		nmglRotatef(angle, 0.707, 0.707, 0);
		nmglTranslatef(150, 150, 0);
		PROFILER_SIZE(amountPolygons2);
		nmglDrawArrays(NMGL_TRIANGLES, 0, 3 * amountPolygons2);
#ifdef __OPEN_GL__
		//angle += 0.03;
		angle += 1.72;
		halSleep(100);
#else
		angle += 1.72;
#endif // __OPEN_GL__


		nmglDisableClientState(NMGL_VERTEX_ARRAY);
		nmglDisableClientState(NMGL_NORMAL_ARRAY);
		counter++;
		if (counter == 256) {
			//PROFILER_PRINT2TBL();
			nmprofiler_disable();
			//return 0;
		}
		//nmglFinish();
		nmglvsSwapBuffer();
	}
	//halFree(vertices_DDR);
	//halFree(vertices_DDR2);
	//halFree(normal_DDR);
	//halFree(normal_DDR2);

#if defined(__GNUC__) && defined(PROFILER0)
	nmprofiler_disable();
#endif // __GNUC__

	nmglvsExit_mc12101();
	return 0x600D600D;
} 

