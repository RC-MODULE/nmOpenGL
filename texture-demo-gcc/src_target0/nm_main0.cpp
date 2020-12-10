#include "nmgl.h"
#include "nmglvs_nmc0.h"
#include "nmgltex_common.h"

float triangles[24]{
	0,0,
	0,31,
	31,31,
	//0,0,
	//0,31,
	//31,31,


	0, 0,
	30, 0,
	30, 30,
	
    //0, 0,
    //30, 0,
    //30, 30,
};

float color[48]{
	1,0,0, 1,
	1,0,0, 1,
	1,0,0, 1,

	1,0,1, 1,
	1,0,1, 1,
	1,0,1, 1,
	
};

float texcoords[12]{
	0.0, 0.0,
	0.0, 1.0,
	1.0, 1.0,
	//0.0, 0.0,
	//0.0, 1.0,
	//1.0, 1.0,
	
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	//0.0, 0.0,
	//1.0, 0.0,
	//1.0, 1.0,



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
extern "C" TexImage2D teximage_earth; //512*512

TexImage2D* texImages[9] = {
							&teximage_256_256,
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
int main()
{
	nmglvsNm0Init();

	nmglClearColor(0, 0, 0.4f, 1.0f);

	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(-768/2, 768 / 2, -768 / 2, 768 / 2, 0, 100);
	
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MIN_FILTER, NMGL_LINEAR);
	nmglTexParameteri(NMGL_TEXTURE_2D, NMGL_TEXTURE_MAG_FILTER, NMGL_LINEAR);
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
	
	nmglEnable(NMGL_TEXTURE_2D);
	
	NMGLfloat texenv_color[4] = {0.0, 0.0, 0.0, 0.0};
	nmglTexEnvfv(NMGL_TEXTURE_ENV, NMGL_TEXTURE_ENV_COLOR, (NMGLfloat*)texenv_color);

	nmglVertexPointer(2, NMGL_FLOAT, 0, triangles);
	nmglColorPointer(4, NMGL_FLOAT, 0, color);
	nmglTexCoordPointer(2, NMGL_FLOAT, 0, texcoords);

	nmglEnableClientState(NMGL_VERTEX_ARRAY);
	nmglEnableClientState(NMGL_COLOR_ARRAY);
	nmglEnableClientState(NMGL_TEXTURE_COORD_ARRAY);
	while(nmglvsNm0Run()){
		nmglClear(NMGL_COLOR_BUFFER_BIT);

		nmglDrawArrays(NMGL_TRIANGLES, 0, 6);

		nmglvsSwapBuffer();
	}
	nmglDisableClientState(NMGL_VERTEX_ARRAY);
	nmglDisableClientState(NMGL_COLOR_ARRAY);
	nmglDisableClientState(NMGL_TEXTURE_COORD_ARRAY);
	nmglvsExit_mc12101();
	return 0x600D600D;
}
