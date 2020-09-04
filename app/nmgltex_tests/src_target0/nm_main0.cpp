#include "nmgl.h"
#include "nmgl_tex_tests.h"
#include "nmgl_tex_tests_config.h"
#include "nmglvs_nmc0.h"
#include <iostream>
extern void* cntxtAddr_nm1;
float triangles[24]{
	0,0,
	0,31,
	31,31,
	
	0, 0,
	30, 0,
	30, 30,
	
};

float color[48]{
	1,0,0, 1,
	1,0,0, 1,
	1,0,0, 1,

	1,0,1, 1,
	1,0,1, 1,
	1,0,1, 1,
	
};

int main()
{
	nmglvsNm0Init();

	nmglClearColor(0, 0, 0.4f, 1.0f);

	nmglMatrixMode(NMGL_PROJECTION);
	nmglOrthof(-768/2, 768 / 2, -768 / 2, 768 / 2, 0, 100);
	

	nmglVertexPointer(2, NMGL_FLOAT, 0, triangles);
	nmglColorPointer(4, NMGL_FLOAT, 0, color);
	nmglEnableClientState(NMGL_VERTEX_ARRAY);
	nmglEnableClientState(NMGL_COLOR_ARRAY);
	/*
	while(nmglvsNm0Run()){
		nmglClear(NMGL_COLOR_BUFFER_BIT);

		nmglDrawArrays(NMGL_TRIANGLES, 0, 6);

		nmglvsSwapBuffer();
	}
	*/
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGLint texture_names[NMGL_MAX_TEX_OBJECTS];
	/*
	printf("Initial nm0 context fields:\n");
	
	printf("firstFreeTexByte=%x\n",cntxt->texState.firstFreeTexByte);
	for(int i=1;i<=NMGL_MAX_TEX_OBJECTS;i++)
	{
		printf("Tex object name=%d\n",i);
		PrintMipMapP(NMGL_Context_NM0,i,cntxt);
	}
	printf("firstFreeTexByte=%x\n",cntxt->texState.firstFreeTexByte);
	
	*/

//=========RUNNING=TESTS======================================================
#ifdef TEST_NMGL_GEN_TEXTURES
	run_nmglGenTextures_test();
#endif

#ifdef TEST_NMGL_BIND_TEXTURE
	run_nmglBindTexture_test();
#endif

#ifdef TEST_NMGL_ACTIVE_TEXTURE
	run_nmglActiveTexture_test();
#endif

#ifdef TEST_NMGL_CLIENT_ACTIVE_TEXTURE
	run_nmglClientActiveTexture_test();
#endif

#ifdef TEST_NMGL_MULTI_TEX_COORD_2F
	run_nmglMultiTexCoord2f_test();
#endif

#ifdef TEST_NMGL_MULTI_TEX_COORD_2FV
	run_nmglMultiTexCoord2fv_test();
#endif

#ifdef TEST_NMGL_TEX_ENV_FV
	run_nmglTexEnvfv_test();
#endif


#ifdef TEST_NMGL_TEX_IMAGE_2D
	run_nmglTexImage2D_test();
#endif

#ifdef TEST_NMGL_TEX_SUB_IMAGE_2D
	run_nmglTexSubImage2D_test();
#endif

#ifdef TEST_NMGL_GET_TEX_ENV_FV
	run_nmglGetTexEnvfv_test();
#endif

#ifdef TESTNMGL_GET_TEX_PARAMETER_IV
	run_nmglGetTexParameteriv_test();
#endif

#ifdef TEST_NMGL_TEX_ENV_I
	run_nmglTexEnvi_test();
#endif

#ifdef TEST_NMGL_GET_TEX_ENV_IV
	run_nmglGetTexEnviv_test();
#endif

#ifdef TEST_NMGL_TEX_PARAMETER_I
	run_nmglTexParameteri_test();
#endif

#ifdef TEST_NMGL_TEX_COORD_POINTER
	run_nmglTexCoordPointer_test();
#endif
while(nmglvsNm0Run()){
		nmglClear(NMGL_COLOR_BUFFER_BIT);

		nmglDrawArrays(NMGL_TRIANGLES, 0, 6);

		nmglvsSwapBuffer();
	}

//=========/RUNNING=TESTS======================================================
	
	nmglDisableClientState(NMGL_VERTEX_ARRAY);
	nmglDisableClientState(NMGL_COLOR_ARRAY);
	nmglvsExit_mc12101();
	return 0x600D600D;
}