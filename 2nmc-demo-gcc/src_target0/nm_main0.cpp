#include "hal_target.h"
#include "hal.h"
#include <time.h>
#include "stdio.h"
#include "nmpp.h"
#include "nmgl.h"
#include "nmglvs_nmc0.h"
#include "nmprofiler.h"
//#include "debugprint.h"
#include "data_NM_32.h"
#include "data_2_sphere.h"


SECTION(".text_shared0") int main()
{
	printf("nm0 start\n");
#if defined(__GNUC__) && defined(DEBUG)
	//nmprofiler_init();
	//nmprofiler_disable();
#endif // __GNUC__

	nmglvsNm0Init();

	setHeap(10);
	//float* vertices_DDR = (float*)halMalloc32(2000 * 12);
	//float* normal_DDR = (float*)halMalloc32(2000 * 12);
	//float* vertices_DDR2 = (float*)halMalloc32(2000 * 12);
	//float* normal_DDR2 = (float*)halMalloc32(2000 * 12);
	int ok;

	int amountPolygons = stl_NM_32_amount;
	float* vertices_DDR = stl_NM_32_vertices;
	float* normal_DDR = stl_NM_32_normals;
	//int amountPolygons = halHostSync(0);
	//sync1
	//halHostSyncAddr(vertices_DDR);
	//sync2
	//halHostSyncAddr(normal_DDR);

	//int amountPolygons2 = halHostSync(0);
	//halHostSyncAddr(vertices_DDR2);

	//sync2
	//halHostSyncAddr(normal_DDR2);
	//ok = halHostSync((int)0x600D600D);
	
	int amountPolygons2 = stl_2_sphere_amount;
	float* vertices_DDR2 = stl_2_sphere_vertices;
	float* normal_DDR2 = stl_2_sphere_normals;

	nmglClearColor(0, 0, 0.4f, 0.0f);

	nmglEnable(NMGL_DEPTH_TEST);
	nmglEnable(NMGL_CULL_FACE);

	nmglMatrixMode(NMGL_MODELVIEW);
	nmglLoadIdentity();
	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(-384, 384, -384, 384, -384, 384);	
	//nmglViewport(128, 128, 384, 384);
	//nmglViewport(0, 0, 256, 256);
	//nmglEnable(NMGL_SCISSOR_TEST);
	//nmglScissor(128, 128, 384, 384);
	//nmglOrthof(-384, 384, -384, 384, -100, 100);
	//nmglFrustumf(-384, 384, -384, 384, 0, 100);
	nmglMatrixMode(NMGL_MODELVIEW);

	nmglEnable(NMGL_LIGHTING);
	nmglEnable(NMGL_LIGHT0);
	float lightVector[4] = { 0, -0.707, 0.707, 0 };
	nmglLightfv(NMGL_LIGHT0, NMGL_POSITION, lightVector);
	float lightDiffuse[4] = { 1, 1, 1, 1 }; 
	nmglLightfv(NMGL_LIGHT0, NMGL_DIFFUSE, lightDiffuse);
	float materialSpec[4] = { 0,0,0,1 };
	float materialDiffuse[4] = { 0,0,0,1 };
	nmglMaterialf(NMGL_FRONT_AND_BACK, NMGL_SHININESS, 15);
	float angle = 0;
	NMGLenum error;
	unsigned time;

	int counter = 0;
	while(nmglvsNm0Run()){
		nmglEnableClientState(NMGL_VERTEX_ARRAY);
		nmglEnableClientState(NMGL_NORMAL_ARRAY);

		nmglClear(NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT);

		nmglVertexPointer(4, NMGL_FLOAT, 0, vertices_DDR);
		nmglNormalPointer(NMGL_FLOAT, 0, normal_DDR);
		nmglMatrixMode(NMGL_MODELVIEW);
		nmglLoadIdentity();
		nmglScalef(0.95f, 0.95f, 0.95f);
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
		nmglMatrixMode(NMGL_MODELVIEW);
		nmglLoadIdentity();
		//nmglScalef(0.95f, 0.95f, 0.95f);
		nmglScalef(0.5f, 0.5f, 0.5f);
		materialDiffuse[0] = 0.4;
		materialDiffuse[1] = 1;
		materialDiffuse[2] = 1;
		materialSpec[0] = 0;
		materialSpec[1] = 0;
		materialSpec[2] = 0;
		nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_DIFFUSE, materialDiffuse);
		nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_SPECULAR, materialSpec);
		nmglRotatef(angle, 0.707, 0.707, 0);
		nmglTranslatef(300, 300, 0);
		PROFILER_SIZE(amountPolygons2);
		nmglDrawArrays(NMGL_TRIANGLES, 0, 3 * amountPolygons2);
		angle += 1.72;
#ifdef __OPEN_GL__
		halSleep(100);
#endif // __OPEN_GL__

		nmglDisableClientState(NMGL_VERTEX_ARRAY);
		nmglDisableClientState(NMGL_NORMAL_ARRAY);
		counter++;
		nmglvsSwapBuffer();
	}

	nmglvsExit_mc12101();
	return 0x600D600D;
} 
