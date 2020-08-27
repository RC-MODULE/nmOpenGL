#if 0
#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"
#include "tests.h"


#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

/*
From OpenGL SC 1.0.1 spec:
//////////////////////////////////////////////////////////////////
Basic restrictions for texturing(only supported items are listed):
//////////////////////////////////////////////////////////////////
Image type:			UNSIGNED_BYTE
Internal format:	RGBA,RGB,LUMINANCE,ALPHA,LUMINANCE_ALPHA


The format must match the base internal format (no conversions from
one format to another during texture image processing are supported.
//////////////////////////////////////////////////////////////////
void nmglTexSubImage2D (
NMGLenum target, 
NMGLint level, 
NMGLint xoffset, 
NMGLint yoffset, 
NMGLsizei width, 
NMGLsizei height, 
NMGLenum format, 
NMGLenum type, 
const void *pixels);
//////////////////////////////////////////////////////////////////

TODO	
*/
extern void fillPixels (void **pixels,NMGLint size);
NMGL_Context_NM0 *cntxt;
extern void* cntxtAddr_nm1;
NMGL_Context_NM1 *nm1_cntxt;

/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
#if 0
/*
int nmglTexSubImage2D_wrongTarget_isError();
int nmglTexSubImage2D_wrongLevel_isError();
int nmglTexSubImage2D_wrongOffsetOrSize_isError();
int nmglTexSubImage2D_wrongFormat_isError();
int nmglTexSubImage2D_wrongType_isError();
int nmglTexSubImage2D_changeSubImage_changeIsCorrect();
*/
#endif
int nmglTexSubImage2D_TexSubImage_contextStateCorrect();
int nmglTexSubImage2D_wrongArgs_isError();
/////////////////////////////////////////////////////////////////////////////////////////////////


//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
//#define USED_SIDE 16 //side in pixels of texture level 0
int run_nmglTexSubImage2D_test()
{
	printf ("\nStart TexSubImage2D tests\n\n");
	cntxt = NMGL_Context_NM0::getContext();
	nm1_cntxt = (NMGL_Context_NM1*)cntxtAddr_nm1;
	RUN_TEST(nmglTexSubImage2D_wrongArgs_isError);
	RUN_TEST(nmglTexSubImage2D_TexSubImage_contextStateCorrect);	
	
	printf ("\nEnd TexSubImage2D tests\n");
	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================


int nmglTexSubImage2D_TexSubImage_contextStateCorrect()
{
	
	void *pixels=NULL;
	void *testarray=NULL;
	int sss=0;
	int i=0;
	int j=0;
	int k=0;
	//int init_size=64;
	int curw=USED_SIDE>>2;
	int curh=USED_SIDE>>2;
	int curB=0;	
	TexImage2D texImages2D[NMGL_MAX_MIPMAP_LVL+1];
	int _accum=0;//for error accumulation
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;

	


	fillMipMap(0,NMGL_RGBA,USED_SIDE,USED_SIDE,0xff);
#ifdef DEBUG
	//printMipMap(0);
#endif
	cntxt->texState.texObjects[0].imageIsSet=1;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	//pixels=calloc(curw*curh,4);
	
	//#define max_level 5
	for(k=0;k<max_level;k++) //each level
	{
		
		for(i=0;i<=(USED_SIDE>>k)-curw;i++)
		{
			for(j=0;j<=(USED_SIDE>>k)-curh;j++)
			{
#ifdef DEBUG
				if((k==0)&&(i<2)&&(j<2))
				{
				DEBUG_PRINT(("_______________________________________\n"));
				DEBUG_PRINT(("Level=%d	startx=%d  starty=%d\n",k,j,i));
				DEBUG_PRINT(("SubWidth=%d	SubHeight=%d \n",curw,curh));
				}
#endif
				copyPixels(ActiveTexObjectP->texImages2D[k].pixels,ActiveTexObjectP->texImages2D[k].internalformat,ActiveTexObjectP->texImages2D[k].width,ActiveTexObjectP->texImages2D[k].height,&testarray);
#ifdef DEBUG
				if((k==0)&&(i<2)&&(j<2))
				{
					printf("Test array:\n");
					printArray32(testarray,ActiveTexObjectP->texImages2D[k].width*ActiveTexObjectP->texImages2D[k].height,16);
				}
#endif

				nmglTexSubImage2D (NMGL_TEXTURE_2D, k, i, j, curw, curh, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
#ifdef DEBUG
				if((k==0)&&(i<2)&&(j<2))
				{
					printf("After sub image:\n");
					printMipMap(0);
				}
			#endif				
				TEST_ASSERT_ACCUM(cntxt->error==NMGL_NO_ERROR);
				TEST_ASSERT_ACCUM(cmpRefreshPixels(ActiveTexObjectP->texImages2D[k].pixels,testarray,pixels,getFormatSize(ActiveTexObjectP->texImages2D[k].internalformat),ActiveTexObjectP->texImages2D[k].width,ActiveTexObjectP->texImages2D[k].height,i,j,curw,curh) == 1 );
				increment(pixels,USED_SIDE*USED_SIDE);
			}
		}
		if(curw > 1) curw>>=1;
		if(curh > 1) curh>>=1;
	}
	//TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	
	

	fillMipMap(1,NMGL_RGBA,USED_SIDE,USED_SIDE,0xAB);
#ifdef DEBUG
	printMipMap(1);
#endif
	cntxt->texState.texObjects[1].imageIsSet=1;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[1];
curw=USED_SIDE>>2;
curh=USED_SIDE>>2;
	memset(pixels,0,(curw*curh*4));

copyPixels(ActiveTexObjectP->texImages2D[0].pixels,ActiveTexObjectP->texImages2D[0].internalformat,ActiveTexObjectP->texImages2D[0].width,ActiveTexObjectP->texImages2D[0].height,&testarray);

nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1,2, curw, curh, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
#ifdef DEBUG
	printf("After sub image:\n");
					printMipMap(1);
#endif
TEST_ASSERT_ACCUM(cntxt->error==NMGL_NO_ERROR);
TEST_ASSERT_ACCUM(cmpRefreshPixels(ActiveTexObjectP->texImages2D[0].pixels,testarray,pixels,getFormatSize(ActiveTexObjectP->texImages2D[0].internalformat),ActiveTexObjectP->texImages2D[0].width,ActiveTexObjectP->texImages2D[0].height,1,2,curw,curh) == 1 );


	TEST_ASSERT(_accum==0);	
	free(pixels);
	free(testarray);
		
	
return 0;
	
}
//-----------------------------------------------------------------
int nmglTexSubImage2D_wrongArgs_isError()
{
	void *pixels;
	int sss=0;
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	_init_tex_obj(0);
	fillMipMap(0,NMGL_RGBA,USED_SIDE,USED_SIDE,0xff);
	//printMipMap(0);
	cntxt->texState.texObjects[0].imageIsSet=1;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	pixels=calloc((USED_SIDE*USED_SIDE)>>4,4);
	if(pixels==0) {DEBUG_PRINT(("Error!Cant allocate mem for test mipmap!\n"));return 2;}
//__________________normal_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_NO_ERROR);

//__________________bad target_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D+1, 0, 1, 1, USED_SIDE>>2,USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_ENUM);
		cntxt->error=NMGL_NO_ERROR;

//__________________bad level_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, -1, 1, 1, USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;	

		nmglTexSubImage2D (NMGL_TEXTURE_2D, NMGL_MAX_MIPMAP_LVL+1, 1, 1, USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;
		
//__________________bad width/height_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, -1, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;		

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, USED_SIDE>>2, -1, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, USED_SIDE, 8, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;		

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, 8, USED_SIDE, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;
			
//__________________bad x/yoffset_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, -1, 1, USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;	

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, -1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, USED_SIDE+1, 1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;	

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, USED_SIDE+1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_VALUE);
		cntxt->error=NMGL_NO_ERROR;
//__________________bad format_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGB, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_OPERATION);
		cntxt->error=NMGL_NO_ERROR;	

//__________________bad type_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1,USED_SIDE>>2, USED_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE+1, pixels);
		TEST_ASSERT_ACCUM(cntxt->error==NMGL_INVALID_ENUM);
free(pixels);
		TEST_ASSERT(sss==0);
		
		//cntxt->error=NMGL_NO_ERROR;			
	/*
	nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const void *pixels)
	*/
		return 0;
}
#endif