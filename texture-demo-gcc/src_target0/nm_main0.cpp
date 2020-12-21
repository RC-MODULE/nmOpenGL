#include "nmgl.h"
#include "nmglvs_nmc0.h"
#include "nmgltex_common.h"

#ifdef __NM__
#define mySleep halSleep
#else
#include <windows.h>
#define mySleep Sleep
#endif 

extern "C" TexImage2D teximage_256_256;
extern "C" TexImage2D teximage_256_256_green_blue;
extern "C" TexImage2D teximage_128_128;
extern "C" TexImage2D teximage_64_64;
extern "C" TexImage2D teximage_32_32;
extern "C" TexImage2D teximage_16_16;
extern "C" TexImage2D teximage_8_8;
extern "C" TexImage2D teximage_4_4;
extern "C" TexImage2D teximage_2_2;
extern "C" TexImage2D teximage_1_1;
extern "C" TexImage2D teximage_mytexture;
extern "C" TexImage2D teximage_earth; //512*512

TexImage2D* texImages[9] = {
							&teximage_256_256_green_blue,
							// &teximage_256_256,
							// &teximage_earth,
							&teximage_128_128,
							&teximage_64_64,
							&teximage_32_32,
							&teximage_16_16,
							&teximage_8_8,
							&teximage_4_4,
							&teximage_2_2,
							&teximage_1_1
						};

// vertices, texcoords, colors, normals
// standaloneAmountPolygons, sideSizeWithGaps
#include "standalone_model.h"

int main()
{
	nmglvsNm0Init();

	nmglClearColor(0, 0, 0.4f, 1.0f);

	//nmglEnable(NMGL_DEPTH_TEST);
	nmglDisable(NMGL_DEPTH_TEST);
	nmglEnable(NMGL_CULL_FACE);

	nmglMatrixMode(NMGL_MODELVIEW);
	nmglLoadIdentity();
	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(-384, 384, -384, 384, -384, 384);
	// nmglFrustumf(-384, 384, -384, 384, 1, 1000);
	nmglMatrixMode(NMGL_MODELVIEW);

	nmglViewport(0, 0, 768, 768);

	//nmglEnable(NMGL_LIGHTING);
	//nmglEnable(NMGL_LIGHT0);
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
	
	nmglTexEnvi(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_MODE, NMGL_ADD);
	
	nmglEnable(NMGL_TEXTURE_2D);
	//nmglDisable(NMGL_TEXTURE_2D);
	
	NMGLfloat texenv_color[4] = {1.0, 0.0, 0.0, 1.0};
	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR, (NMGLfloat*)texenv_color);

	nmglEnableClientState(NMGL_VERTEX_ARRAY);
	nmglEnableClientState(NMGL_COLOR_ARRAY);
    nmglEnableClientState(NMGL_NORMAL_ARRAY);
	nmglEnableClientState(NMGL_TEXTURE_COORD_ARRAY);

	unsigned int vertexCountInTriangle = 3;

	unsigned int startQuadNumber = 0; 
	unsigned int startTriangleNumber = startQuadNumber * 2;
	unsigned int startVertexNumber = startTriangleNumber * vertexCountInTriangle;
	unsigned int showQuadAmount = standaloneAmountPolygons/2; 
	unsigned int showTriangleAmount = showQuadAmount * 2;
	
	float startAngle = 21.0f;
	float angleDelta = 0.0f;
	int rotate = 1; //1 if rotate, 0 - if static

	if (rotate) {
#ifdef __NM__
		angleDelta = 1.0f;
#else
		angleDelta = 1.0f;
#endif
	} else {
		angleDelta = 0.0f;
	}
	int delayMilliseconds = 0;

	float* curVertices;
	float* curColors;
	float* curNormals;
	float* curTexcoords;

	angle = startAngle;
	while(nmglvsNm0Run()){
		curVertices = &vertices_standalone[startVertexNumber * 4];
		curNormals = &normals_standalone[startVertexNumber * 3];
		curTexcoords = &texcoords_standalone[startVertexNumber * 2];
		curColors = &colors_standalone[startVertexNumber * 4];

		nmglVertexPointer(4, NMGL_FLOAT, 0, curVertices);
		nmglNormalPointer(NMGL_FLOAT, 0, curNormals);
		nmglTexCoordPointer(2, NMGL_FLOAT, 0, curTexcoords);
		nmglColorPointer(4, NMGL_FLOAT, 0, curColors);
		nmglClear(NMGL_COLOR_BUFFER_BIT | NMGL_DEPTH_BUFFER_BIT);
		
		nmglLoadIdentity();
		nmglScalef (0.7,0.7,0.7);
		nmglTranslatef(-sideSizeWithGaps/2.0,-sideSizeWithGaps/2.0,0.0f);
		nmglTranslatef(sideSizeWithGaps/2.0,sideSizeWithGaps/2.0,0.0f);
		nmglRotatef(angle, 0.0f, 0.0f, 1.0f);
		nmglTranslatef(-sideSizeWithGaps/2.0,-sideSizeWithGaps/2.0,0.0f);
		
		nmglDrawArrays(NMGL_TRIANGLES, 0, 3 * showTriangleAmount);
		nmglvsSwapBuffer();

		mySleep(delayMilliseconds);
		angle += angleDelta;
	}

	nmglDisableClientState(NMGL_VERTEX_ARRAY);
	nmglDisableClientState(NMGL_COLOR_ARRAY);
	nmglDisableClientState(NMGL_TEXTURE_COORD_ARRAY);
	nmglvsExit_mc12101();
	return 0x600D600D;
}
