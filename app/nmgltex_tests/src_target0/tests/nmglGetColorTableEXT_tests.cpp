#include "demo3d_nm0.h"
#include "tests.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

//extern NMGLubyte mem_palette[NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*NMGL_MAX_TEX_OBJECTS];
//extern NMGLubyte data[NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE];

/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglGetColorTableEXT (NMGLenum target, NMGLenum format, NMGLenum type,void *data);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglGetColorTableEXT_wrongArgs_isError();
int nmglGetColorTableEXT_setColorTable_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglGetColorTableEXT_test()
{
	
	
	printf ("\nStart nmglGetColorTableEXT tests\n\n");
		RUN_TEST(nmglGetColorTableEXT_wrongArgs_isError);
		RUN_TEST(nmglGetColorTableEXT_setColorTable_contextCorrect);
	printf ("\nEnd nmglGetColorTableEXT tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================


// 
int nmglGetColorTableEXT_wrongArgs_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();	
	void *data=cntxt->texState.palette_pointers[NMGL_MAX_TEX_OBJECTS];	
	
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	nmglGetColorTableEXT (NMGL_TEXTURE_2D+1, NMGL_RGBA, NMGL_UNSIGNED_BYTE,data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
	cntxt->error=NMGL_NO_ERROR;
	nmglGetColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA+1, NMGL_UNSIGNED_BYTE,data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
	cntxt->error=NMGL_NO_ERROR;
	nmglGetColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA, NMGL_UNSIGNED_BYTE+1,data);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
	cntxt->error=NMGL_NO_ERROR;
	nmglGetColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA, NMGL_UNSIGNED_BYTE,data);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	nmglGetColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA, NMGL_UNSIGNED_BYTE,data);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	nmglGetColorTableEXT (NMGL_SHARED_TEXTURE_PALETTE_EXT, NMGL_RGBA, NMGL_UNSIGNED_BYTE,data);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);

	return 0;
}
//------------------------------------------------------------------------------
// 
int nmglGetColorTableEXT_setColorTable_contextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	//NMGLubyte  data1[16]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x8,0x10,0x11,0x12,0x13,0x14,0x15,0x16};	
	NMGLubyte* data1=cntxt->texState.palette_pointers[NMGL_MAX_TEX_OBJECTS];
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	int i=0;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	*cntxt->texState.paletts_widths_pointers[0] = NMGL_MAX_PALETTE_WIDTH;
	*cntxt->texState.paletts_widths_pointers[1] = NMGL_MAX_PALETTE_WIDTH;

	//NMGLubyte* init_pointer;
	//cntxt->texState.texObjects[0].palette.setColors(&data1[0]);
	for (int i=0;i<NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		*cntxt->texState.palette_pointers[0]=i;
		*cntxt->texState.palette_pointers[1]=i+2;
	}
	
	nmglGetColorTableEXT (NMGL_TEXTURE_2D, NMGL_RGBA, NMGL_UNSIGNED_BYTE,data1);
	for(int i=0;i<NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		if(*((NMGLubyte*)data1+i) != *((NMGLubyte*)ActiveTexObjectP->palette.colors+i))
		{
			TEST_ASSERT(0);
		}
	}	
	TEST_ASSERT(1);
	nmglGetColorTableEXT (NMGL_SHARED_TEXTURE_PALETTE_EXT, NMGL_RGBA, NMGL_UNSIGNED_BYTE,data1);
	for(int i=0;i<NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		if(*((NMGLubyte*)data1+i) != *((NMGLubyte*)cntxt->texState.palette_pointers[0]+i))
		{
			TEST_ASSERT(0);
		}
	}	
	TEST_ASSERT(1);

return 0;
}
