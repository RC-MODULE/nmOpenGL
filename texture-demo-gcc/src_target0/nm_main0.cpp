#include "hal_target.h"
#include "hal.h"
#include <time.h>
#include "stdio.h"
#include "nmpp.h"
#include "nmgl.h"
#include "nmglvs_nmc0.h"
	
#pragma data_section ".data_shared"
//SECTION(".data_shared") float vertices_DDR[2000 * 12];
//SECTION(".data_shared") float normal_DDR[2000 * 9];
//SECTION(".data_shared") float vertices_DDR2[2000 * 12];
//SECTION(".data_shared") float normal_DDR2[2000 * 9];
SECTION(".data_shared") float twoTriangles[24 * 4] = {
	110, 90, 0, 1,
	90, 110, 0, 1,
	90, 90, 0, 1,
	110, 110, 0, 1,

	110, 90, 0, 1,
	90, 90, 0, 1,
	110, 90, 0, 1,
	110, 110, 0, 1,

	110, 90, 0, 1,
	125, 95, 0, 1,
	90, 90, 0, 1,
	75, 95, 0, 1,

	90, 90, 0, 1,
	75, 95, 0, 1,
	90, 90, 0, 1,
	75, 95, 0, 1,
	110, 90, 0, 1,
	125, 95, 0, 1,
	110, 90, 0, 1,
	125, 95, 0, 1,
	110, 90, 0, 1,
	125, 95, 0, 1,
	110, 90, 0, 1,
	125, 95, 0, 1,
};

extern "C" TexImage2D teximage_256_256;
extern "C" TexImage2D teximage_128_128;
extern "C" TexImage2D teximage_64_64;
extern "C" TexImage2D teximage_32_32;
extern "C" TexImage2D teximage_16_16;
extern "C" TexImage2D teximage_8_8;
extern "C" TexImage2D teximage_4_4;
extern "C" TexImage2D teximage_2_2;
extern "C" TexImage2D teximage_1_1;
extern "C" TexImage2D teximage_mytexture;

TexImage2D* texImages[9] = {&teximage_256_256,
							&teximage_128_128,
							&teximage_64_64,
							&teximage_32_32,
							&teximage_16_16,
							&teximage_8_8,
							&teximage_4_4,
							&teximage_2_2,
							&teximage_1_1
						};

#include "standalone_model.h"

SECTION(".text_shared") int main()
{
	nmglvsNm0Init();
	setHeap(10);
	float* vertices_DDR = (float*)halMalloc32(2000 * 12);
	float* normal_DDR = (float*)halMalloc32(2000 * 9);
	//float* vertices_DDR2 = (float*)halMalloc32(2000 * 12);
	//float* normal_DDR2 = (float*)halMalloc32(2000 * 9);
	
#ifdef __OPEN_GL__
	Models models;
	//char* filePath = models.nm;
	char* filePath = models.snowman;

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
	fmodel = fopen(filePath, "r");
	createArrayVec4(fmodel, vertices_DDR2, normal_DDR2, 0.5);
	


	int amountPolygons2 = amount;
	int ok;
#else
	int amountPolygons = halHostSync(0);

	//Массив цветов полигонов (по 4 компоненты на вершину)
	//sync1
	halHostSync((int)vertices_DDR);

	//sync2
	int ok = halHostSync((int)normal_DDR);

	// int amountPolygons2 = halHostSync(0);
	// halHostSync((int)vertices_DDR2);

	// //sync2
	// ok = halHostSync((int)normal_DDR2);
	ok = halHostSync((int)0x600D600D);

//This parameters are in standalone_model.h include file:
// * float vertices_standalone[] = {};
// * float normals_standalone[] = {};
// * float texcoords_standalone[] = {};
// * amountPolygons = 16 ;

// amountPolygons = standaloneAmountPolygons;

#endif
	
	nmglClearColor(0, 0, 0.4, 0.0f);
	//nmglClearColor(0.5, 0.5, 0.5, 1.0f);

	nmglEnable(NMGL_DEPTH_TEST);
	nmglEnable(NMGL_CULL_FACE);

	nmglMatrixMode(NMGL_MODELVIEW);
	nmglLoadIdentity();
	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(-384, 384, -384, 384, -384, 384);
	// nmglFrustumf(-384, 384, -384, 384, 1, 1000);
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
	float angle = 0;
	NMGLenum error;
	unsigned time;
	
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MIN_FILTER, NMGL_NEAREST);
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MAG_FILTER, NMGL_NEAREST);
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_S, NMGL_REPEAT);
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_WRAP_T, NMGL_REPEAT);

	//glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	nmglTexImage2D(NMGL_TEXTURE_2D, 0,NMGL_RGB, texImages[0]->width, texImages[0]->height, 0, NMGL_RGB, NMGL_UNSIGNED_BYTE, texImages[0]->pixels);
	nmglTexImage2D(NMGL_TEXTURE_2D, 1,NMGL_RGB, texImages[1]->width, texImages[1]->height, 0, NMGL_RGB, NMGL_UNSIGNED_BYTE, texImages[1]->pixels);
	nmglTexImage2D(NMGL_TEXTURE_2D, 2,NMGL_RGB, texImages[2]->width, texImages[2]->height, 0, NMGL_RGB, NMGL_UNSIGNED_BYTE, texImages[2]->pixels);
	nmglTexImage2D(NMGL_TEXTURE_2D, 3,NMGL_RGB, texImages[3]->width, texImages[3]->height, 0, NMGL_RGB, NMGL_UNSIGNED_BYTE, texImages[3]->pixels);
	nmglTexImage2D(NMGL_TEXTURE_2D, 4,NMGL_RGB, texImages[4]->width, texImages[4]->height, 0, NMGL_RGB, NMGL_UNSIGNED_BYTE, texImages[4]->pixels);
	nmglTexImage2D(NMGL_TEXTURE_2D, 5,NMGL_RGB, texImages[5]->width, texImages[5]->height, 0, NMGL_RGB, NMGL_UNSIGNED_BYTE, texImages[5]->pixels);
	nmglTexImage2D(NMGL_TEXTURE_2D, 6,NMGL_RGB, texImages[6]->width, texImages[6]->height, 0, NMGL_RGB, NMGL_UNSIGNED_BYTE, texImages[6]->pixels);
	nmglTexImage2D(NMGL_TEXTURE_2D, 7,NMGL_RGB, texImages[7]->width, texImages[7]->height, 0, NMGL_RGB, NMGL_UNSIGNED_BYTE, texImages[7]->pixels);
	nmglTexImage2D(NMGL_TEXTURE_2D, 8,NMGL_RGB, texImages[8]->width, texImages[8]->height, 0, NMGL_RGB, NMGL_UNSIGNED_BYTE, texImages[8]->pixels);
	
	nmglTexEnvi(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_MODE, NMGL_REPLACE);
	
	NMGLfloat texenv_color[4] = {0.0, 0.0, 0.0, 0.0};
	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR, (NMGLfloat*)texenv_color);
	
	nmglEnable(NMGL_TEXTURE_2D);
//	nmglDisable(NMGL_TEXTURE_2D);
	
	while(nmglvsNm0Run()){
		nmglEnableClientState(NMGL_VERTEX_ARRAY);
		nmglEnableClientState(NMGL_NORMAL_ARRAY);
		nmglEnableClientState(NMGL_TEXTURE_COORD_ARRAY);

		nmglClear(NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT);
		//nmglTranslatef(1, 0, 0);
		//nmglVertexPointer(4, NMGL_FLOAT, 0, twoTriangles);
		//nmglDrawArrays(NMGL_LINES, 0, 24);

		nmglVertexPointer(4, NMGL_FLOAT, 0, vertices_DDR);
		nmglNormalPointer(NMGL_FLOAT, 0, normal_DDR);
		// nmglVertexPointer(4, NMGL_FLOAT, 0, vertices_standalone);
		// nmglNormalPointer(NMGL_FLOAT, 0, normals_standalone);
		nmglTexCoordPointer(2, NMGL_FLOAT, 0, texcoords_standalone);

		nmglLoadIdentity();
		// nmglTranslatef(0.0f, 0.0f, 200.0f);
		nmglScalef(1.0f, 1.0f, 1.0f);
		nmglTranslatef(0.0f, 0.0f, 0.0f);
		nmglRotatef(angle, 0.707f, 0.707f, 0);
		materialDiffuse[0] = 1;
		materialDiffuse[2] = 0;
		nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_DIFFUSE, materialDiffuse);
		materialSpec[0] = 0.6;
		materialSpec[1] = 0.6;
		materialSpec[2] = 0.6;
		nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_SPECULAR, materialSpec);
		nmglDrawArrays(NMGL_TRIANGLES, 0, 3 * amountPolygons);

		// nmglVertexPointer(4, NMGL_FLOAT, 0, vertices_DDR2);
		// nmglNormalPointer(NMGL_FLOAT, 0, normal_DDR2);
		// nmglLoadIdentity();
		// materialDiffuse[0] = 0.4;
		// materialDiffuse[1] = 1;
		// materialDiffuse[2] = 1;
		// materialSpec[0] = 0;
		// materialSpec[1] = 0;
		// materialSpec[2] = 0;
		// nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_DIFFUSE, materialDiffuse);
		// nmglMaterialfv(NMGL_FRONT_AND_BACK, NMGL_SPECULAR, materialSpec);
		// nmglRotatef(angle, 0.707, 0.707, 0);
		// nmglTranslatef(150, 150, 0);
		// nmglDrawArrays(NMGL_TRIANGLES, 0, 3 * amountPolygons2);
#ifdef __OPEN_GL__
		angle += 0.03;
#else
		angle += 1.72;
#endif // __OPEN_GL__

		nmglDisableClientState(NMGL_VERTEX_ARRAY);
		nmglDisableClientState(NMGL_NORMAL_ARRAY);

		//nmglFinish();
		nmglvsSwapBuffer();
	}
	halFree(vertices_DDR);
	//halFree(vertices_DDR2);
	halFree(normal_DDR);
	//halFree(normal_DDR2);
	nmglvsExit_mc12101();
	return 0x600D600D;
} 

