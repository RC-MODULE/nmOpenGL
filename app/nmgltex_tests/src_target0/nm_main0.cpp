#include "nmgl.h"
#include "nmgl_tex_tests.h"
#include "nmgl_tex_tests_config.h"
#include "nmglvs_nmc0.h"
#include <iostream>
//#include "time.h"
#include "demo3d_nm1.h"
extern void* cntxtAddr_nm1;
NMGL_Context_NM1 *cntxt_nm1;
unsigned int nmpu1IsAccessible;

#define tstdtl 5
//#define ENABLE_STATISTICS
#ifdef ENABLE_STATISTICS
	
	SECTION(".data_imu0") volatile int imu0datai[tstdtl];
	SECTION(".data_shmem0") volatile  int shmemdatai[tstdtl];
	SECTION(".data_DDR") volatile  int ddrdatai[tstdtl];
#endif
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
//#if 0
void make_mem_statistics()
{
	
	//#define TSTDATA(x,y,z) SECTION(x) y z[tstdtl]

	
	float ave_ddr_time=0.0;
	float ave_imu_time=0.0;
	float ave_shmem_time=0.0;
	int i=0;
	float frz=0.0;
	volatile int irz=0;
	long sum=0;
	volatile clock_t clks=0;
	volatile clock_t clks1=0;
	volatile int diff=0;

	
	for (i=1;i<=tstdtl;i++)
	{
		clks=clock();				
		irz=imu0datai[i-1];
		clks1=clock();
		diff=clks1-clks;
		printf("clks=%d clks1=%d\tdiff=%d\n",(int)clks,(int)clks1,(int)diff);
		sum+=diff;
		ave_imu_time=sum/(float)i;
		printf("ave_imu_time=%f\n",ave_imu_time);
	}
	sum=0;
	ave_imu_time=0;
	printf("\n shmem int\n");
	for (i=1;i<=tstdtl;i++)
	{
		clks=clock();		
		irz=shmemdatai[i-1];
		clks1=clock();
		diff=clks1-clks;
		printf("clks=%d clks1=%d\tdiff=%d\n",(int)clks,(int)clks1,(int)diff);
	
		sum+=diff;
		ave_shmem_time=sum/(float)i;
		printf("ave_shmem_time=%f\n",ave_shmem_time);
	}
	sum=0;
	ave_shmem_time=0;
	printf("\n DDR int\n");
	for (i=1;i<=tstdtl;i++)
	{
		clks=clock();		
		irz=ddrdatai[i-1];
		clks1=clock();
		diff=clks1-clks;
		printf("clks=%d clks1=%d\tdiff=%d\n",(int)clks,(int)clks1,(int)diff);
		
		sum+=diff;
		ave_ddr_time=sum/(float)i;
		printf("ave_ddr_time=%f\n",ave_ddr_time);
	}

}
//#endif
int main()
{
	#ifdef ENABLE_STATISTICS
	make_mem_statistics();
	#endif
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
	clock_t clks=0;
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
printf("\n***** Start nmgl texture functions tests *****\n");
cntxt_nm1 = (NMGL_Context_NM1*)cntxtAddr_nm1;
	nmpu1IsAccessible = 0;
	DEBUG_PRINT(("Context nm1=%x\n",cntxt_nm1));
	if(cntxt_nm1 == 0) {
		printf("\nWarning: NM1 context address is equal to 0. \nNmpu1 is not accessible from nmpu0. Test only for nmpu0.\n");
	} 
	else if (cntxt_nm1->texState.refValue != 0xC0DEC0DE) {
		printf("\nWarning: refValue field from nmpu1 texture context is not equal to 0xCODECODE.\nNmpu1 is not accessible from nmpu0. Test only for nmpu0.\n");
	}
	else {
		nmpu1IsAccessible = 1;
	}
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
	#ifdef ENABLE_STATISTICS
		clks=clock();
		printf("clks before=0x%x\n",(int)clks);
	#endif
	run_nmglTexSubImage2D_test();
	#ifdef ENABLE_STATISTICS
		clks=clock();
		printf("clks after=0x%x\n",(int)clks);
	#endif
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
printf("\n***** End nmgl texture functions tests *****\n");
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