#if 0
#include "demo3d_nm0.h"
#include "nmgldef.h"
#include "nmgltype.h"
#include "nmgl_data0.h"

#include "tex_common.h"
#include "tex_functions.h"
#include  "tex_support_functions.h"
#include  "tex_test_support_functions.h"

#include "tests.h"

#include <iostream>
#include <cstring>


//#undef DEBUG
//#define DEBUG

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
#define INIT_SIDE 16 //side in pixels of texture level 0
int run_nmglTexSubImage2D_test()
{
	printf ("\nStart TexSubImage2D tests\n\n");
	
	RUN_TEST(nmglTexSubImage2D_TexSubImage_contextStateCorrect);
	RUN_TEST(nmglTexSubImage2D_wrongArgs_isError);
	
	printf ("\nEnd TexSubImage2D tests\n");
	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================
//#define ActiveTexObject
#define TEST_ASSERT_ACCUM(x) \
if(!(x)) \
{ \
sss++;\
}

int nmglTexSubImage2D_TexSubImage_contextStateCorrect()
{
	
	void *pixels=NULL;
	void *testarray=NULL;
	int sss=0;
	int i=0;
	int j=0;
	int k=0;
	//int init_size=64;
	int curw=INIT_SIDE>>2;
	int curh=INIT_SIDE>>2;
	int curB=0;	
	TexImage2D texImages2D[NMGL_MAX_MIPMAP_LEVEL+1];
	int _accum=0;//for error accumulation
	cntxt.error=NMGL_NO_ERROR;
	cntxt.activeTexUnit=NMGL_TEXTURE0;
	cntxt.activeTexUnitIndex=0;

	_init_tex_obj(0);


	fillMipMap(0,NMGL_RGBA,INIT_SIDE,INIT_SIDE,0xff);
#ifdef DEBUG
	printMipMap(0);
#endif
	cntxt.texObjects[0].imageIsSet=1;
	cntxt.texUnits[0].boundTexObject=&cntxt.texObjects[0];
	pixels=calloc(curw*curh,4);
	if(pixels==0) {DEBUG_PRINT(("Error!Cant allocate mem for test mipmap!\n"));return 2;}
	#define max_level 5
	for(k=0;k<max_level;k++) //each level
	{
		
		for(i=0;i<=(INIT_SIDE>>k)-curw;i++)
		{
			for(j=0;j<=(INIT_SIDE>>k)-curh;j++)
			{
#ifdef DEBUG
				if((k==0)&&(i<2)&&(j<2))
				{
				DEBUG_PRINT(("_______________________________________\n"));
				DEBUG_PRINT(("Level=%d	startx=%d  starty=%d\n",k,j,i));
				DEBUG_PRINT(("SubWidth=%d	SubHeight=%d \n",curw,curh));
				}
#endif
				copyPixels(ActiveTexObject->texImages2D[k].pixels,ActiveTexObject->texImages2D[k].internalformat,ActiveTexObject->texImages2D[k].width,ActiveTexObject->texImages2D[k].height,&testarray);
#ifdef DEBUG
				if((k==0)&&(i<2)&&(j<2))
				{
					printf("Test array:\n");
					printArray32(testarray,ActiveTexObject->texImages2D[k].width*ActiveTexObject->texImages2D[k].height,16);
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
				TEST_ASSERT_ACCUM(cntxt.error==NMGL_NO_ERROR);
				TEST_ASSERT_ACCUM(cmpRefreshPixels(ActiveTexObject->texImages2D[k].pixels,testarray,pixels,getFormatSize(ActiveTexObject->texImages2D[k].internalformat),ActiveTexObject->texImages2D[k].width,ActiveTexObject->texImages2D[k].height,i,j,curw,curh) == 1 );
				increment(pixels,INIT_SIDE*INIT_SIDE);
			}
		}
		if(curw > 1) curw>>=1;
		if(curh > 1) curh>>=1;
	}
	//TEST_ASSERT(cntxt.error==NMGL_NO_ERROR);
	
	

	fillMipMap(1,NMGL_RGBA,INIT_SIDE,INIT_SIDE,0xAB);
#ifdef DEBUG
	printMipMap(1);
#endif
	cntxt.texObjects[1].imageIsSet=1;
	cntxt.texUnits[0].boundTexObject=&cntxt.texObjects[1];
curw=INIT_SIDE>>2;
curh=INIT_SIDE>>2;
	memset(pixels,0,(curw*curh*4));

copyPixels(ActiveTexObject->texImages2D[0].pixels,ActiveTexObject->texImages2D[0].internalformat,ActiveTexObject->texImages2D[0].width,ActiveTexObject->texImages2D[0].height,&testarray);

nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1,2, curw, curh, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
#ifdef DEBUG
	printf("After sub image:\n");
					printMipMap(1);
#endif
TEST_ASSERT_ACCUM(cntxt.error==NMGL_NO_ERROR);
TEST_ASSERT_ACCUM(cmpRefreshPixels(ActiveTexObject->texImages2D[0].pixels,testarray,pixels,getFormatSize(ActiveTexObject->texImages2D[0].internalformat),ActiveTexObject->texImages2D[0].width,ActiveTexObject->texImages2D[0].height,1,2,curw,curh) == 1 );


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
	cntxt.error=NMGL_NO_ERROR;
	cntxt.activeTexUnit=NMGL_TEXTURE0;
	cntxt.activeTexUnitIndex=0;
	_init_tex_obj(0);
	fillMipMap(0,NMGL_RGBA,INIT_SIDE,INIT_SIDE,0xff);
	//printMipMap(0);
	cntxt.texObjects[0].imageIsSet=1;
	cntxt.texUnits[0].boundTexObject=&cntxt.texObjects[0];
	pixels=calloc((INIT_SIDE*INIT_SIDE)>>4,4);
	if(pixels==0) {DEBUG_PRINT(("Error!Cant allocate mem for test mipmap!\n"));return 2;}
//__________________normal_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, INIT_SIDE>>2, INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_NO_ERROR);

//__________________bad target_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D+1, 0, 1, 1, INIT_SIDE>>2,INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_ENUM);
		cntxt.error=NMGL_NO_ERROR;

//__________________bad level_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, -1, 1, 1, INIT_SIDE>>2, INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;	

		nmglTexSubImage2D (NMGL_TEXTURE_2D, NMGL_MAX_MIPMAP_LEVEL+1, 1, 1, INIT_SIDE>>2, INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;
		
//__________________bad width/height_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, -1, INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;		

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, INIT_SIDE>>2, -1, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, INIT_SIDE, 8, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;		

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1, 8, INIT_SIDE, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;
			
//__________________bad x/yoffset_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, -1, 1, INIT_SIDE>>2, INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;	

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, -1,INIT_SIDE>>2, INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, INIT_SIDE+1, 1,INIT_SIDE>>2, INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;	

		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, INIT_SIDE+1,INIT_SIDE>>2, INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_VALUE);
		cntxt.error=NMGL_NO_ERROR;
//__________________bad format_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1,INIT_SIDE>>2, INIT_SIDE>>2, NMGL_RGB, NMGL_UNSIGNED_BYTE, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_OPERATION);
		cntxt.error=NMGL_NO_ERROR;	

//__________________bad type_________________________________________________
		nmglTexSubImage2D (NMGL_TEXTURE_2D, 0, 1, 1,INIT_SIDE>>2, INIT_SIDE>>2, NMGL_RGBA, NMGL_UNSIGNED_BYTE+1, pixels);
		TEST_ASSERT_ACCUM(cntxt.error==NMGL_INVALID_ENUM);
free(pixels);
		TEST_ASSERT(sss==0);
		
		//cntxt.error=NMGL_NO_ERROR;			
	/*
	nmglTexSubImage2D (NMGLenum target, NMGLint level, NMGLint xoffset, NMGLint yoffset, NMGLsizei width, NMGLsizei height, NMGLenum format, NMGLenum type, const void *pixels)
	*/
		return 0;
}

#endif