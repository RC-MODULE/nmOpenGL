#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "nmgl_tex_test_common.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif
extern NMGL_Context_NM1 *cntxt_nm1;
extern unsigned int nmpu1IsAccessible;
extern NMGLubyte mem_palette [NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*NMGL_MAX_TEX_OBJECTS];

extern void *data;   


/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglColorTableEXT (NMGLenum target, NMGLenum internalformat, NMGLsizei width, NMGLenum format, NMGLenum type, const void *data);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglColorTableEXT_wrongArgs_isError();
int nmglColorTableEXT_setColorTable_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglColorTableEXT_test()
{
	
	//std::cout << "Hello";
	printf ("\nStart nmglColorTableEXT tests\n\n");	
		RUN_TEST(nmglColorTableEXT_wrongArgs_isError);
		RUN_TEST(nmglColorTableEXT_setColorTable_contextCorrect);
	printf ("\nEnd nmglColorTableEXT tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================


// 
int nmglColorTableEXT_wrongArgs_isError()
{
	
	//printf("Values[0]=%d",values[0]);
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	
	nmglColorTableEXT (NMGL_TEXTURE_2D+1, NMGL_RGBA, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
cntxt->error=NMGL_NO_ERROR;
	nmglColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA+1, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;
	nmglColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA+1, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA+1, NMGL_UNSIGNED_BYTE, data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;
	nmglColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA+1, NMGL_UNSIGNED_BYTE, data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;
	nmglColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA, NMGL_MAX_PALETTE_WIDTH+1, NMGL_RGBA,NMGL_UNSIGNED_BYTE,data);
	TEST_ASSERT(cntxt->error==NMGL_OUT_OF_MEMORY);
cntxt->error=NMGL_NO_ERROR;
	nmglColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA ,0, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;
	nmglColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA ,-1, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;	
	nmglColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA ,-1, NMGL_RGBA, NMGL_UNSIGNED_BYTE+1, data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
cntxt->error=NMGL_NO_ERROR;	
	nmglColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	return 0;
}
//------------------------------------------------------------------------------
// 
int nmglColorTableEXT_setColorTable_contextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	int i=0;
	NMGLsizei cwidth=NMGL_MAX_PALETTE_WIDTH;
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];

	for (i=0;i<cwidth*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		*((NMGLubyte*)data+i)=0x1+i;
	}
	nmglColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA, cwidth, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data);
	wait_for_nm1_if_available;
DEBUG_PRINT(("cwidth=%d\n0width=%d\n1width=%d\n",cwidth,cntxt->texState.texObjects[0].palette.width,cntxt_nm1->texState.texObjects[0].palette.width));
	TEST_ASSERT(cntxt->texState.texObjects[0].palette.width == cwidth);
	if(nmpu1IsAccessible == 1) {TEST_ASSERT(cntxt_nm1->texState.texObjects[0].palette.width == cwidth);}
	
	for (i=0;i<cwidth*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		if(*((NMGLubyte*)data+i) != *((NMGLubyte*)(cntxt->texState.texObjects[0].palette.colors+i)))
		{
			
			printf("i=%d\ndata[%d]=%x pale[%d]=%d data_nm1[%d]\n",i,i,*((NMGLubyte*)data+i),i,*((NMGLubyte*)(cntxt->texState.texObjects[0].palette.colors+i)),i,*((NMGLubyte*)(cntxt_nm1->texState.texObjects[0].palette.colors+i)));
			TEST_ASSERT(0);
		}
		if(nmpu1IsAccessible == 1)
		{
			if(*((NMGLubyte*)data+i) != *((NMGLubyte*)(cntxt_nm1->texState.texObjects[0].palette.colors+i)))
			{
					printf("i=%d\n pale[%d]=%d data_nm1[%d]\n",i,i,*((NMGLubyte*)(cntxt->texState.texObjects[0].palette.colors+i)),i,*((NMGLubyte*)(cntxt_nm1->texState.texObjects[0].palette.colors+i)));
				TEST_ASSERT(0);
			}
		}
	}
	TEST_ASSERT(1);
return 0;
}
