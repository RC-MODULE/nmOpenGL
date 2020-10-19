#include "demo3d_nm0.h"
#include "nmgl_tex_test_common.h"
#include "nmgltex_common.h"

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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglColorSubTableEXT (NMGLenum target, NMGLsizei start, NMGLsizei count, NMGLenum format, NMGLenum type, const void *data);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
*/
extern NMGLubyte mem_palette [NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*NMGL_MAX_TEX_OBJECTS];
void *data=&mem_palette[(int)(NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*(NMGL_MAX_TEX_OBJECTS-1))];   
//////////////////////////////////////////////////////////////////////////////////////////////
int check_palette(NMGL_Context_NM0 *cntxt,NMGLsizei start,NMGLsizei count,const void* data);
void inc_palette(void* data,NMGLsizei curw);
///
//Тестовые сценарии
int nmglColorSubTableEXT_wrongArgs_isError();
int nmglColorSubTableEXT_setContext_ContextCorrect();
//----------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////////////////////////

int check_palette(NMGL_Context_NM0 *cntxt,NMGLsizei start,NMGLsizei count,const void* data)
{
	int i=0;
	for (i=0;i<count*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		if (start+i/RGBA_TEXEL_SIZE_UBYTE >= ActiveTexObjectP->palette.width) break;
		if(*((NMGLubyte*)ActiveTexObjectP->palette.colors+start*RGBA_TEXEL_SIZE_UBYTE+i) != *((NMGLubyte*)data+i)) return 0;
	}
return 1;//match
}
void inc_palette(void* data,NMGLsizei curw)
{
	int i=0;
	for(i=0;i<curw;i++)
	{
		if(i>=NMGL_MAX_PALETTE_WIDTH) break;
		*((NMGLubyte*)data+i)+=1;
	}
}

//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglColorSubTableEXT_test()
{
	

	printf ("\nStart nmglColorSubTableEXT tests\n\n");
		RUN_TEST(nmglColorSubTableEXT_wrongArgs_isError);
		RUN_TEST(nmglColorSubTableEXT_setContext_ContextCorrect);
	printf ("\nEnd nmglColorSubTableEXT tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================


// 
int nmglColorSubTableEXT_wrongArgs_isError()
{

	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];

nmglColorSubTableEXT (NMGL_TEXTURE_2D+1, 0, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE ,data);
TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, -1, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE ,data);
TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, NMGL_MAX_PALETTE_WIDTH, 0, NMGL_RGBA,NMGL_UNSIGNED_BYTE ,data);
TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, NMGL_MAX_PALETTE_WIDTH, 1, NMGL_RGBA,NMGL_UNSIGNED_BYTE ,data);
TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, 0, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA+1,NMGL_UNSIGNED_BYTE ,data);
TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, 0, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE+1 ,data);
TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
cntxt->error=NMGL_NO_ERROR;
nmglColorSubTableEXT (NMGL_TEXTURE_2D, 0, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE,data);
TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	
	return 0;
}
//------------------------------------------------------------------------------
// 
int nmglColorSubTableEXT_setContext_ContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	NMGLsizei  start=0,i;
	NMGLsizei curw=(NMGL_MAX_PALETTE_WIDTH)>>2;
	ActiveTexObjectP->palette.width=NMGL_MAX_PALETTE_WIDTH;
	ActiveTexObjectP->palette.colors=(NMGLubyte*)(mem_palette+ActiveTexObjectP->name*NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE);//delete
	for(i=0;i<NMGL_MAX_PALETTE_WIDTH;i++)
	{
		*((NMGLubyte*)ActiveTexObjectP->palette.colors+i)=0x1+i;
		*((NMGLubyte*)data+i)=0x0;
	}
	for(i=0,start=0;i<NMGL_MAX_PALETTE_WIDTH*3;i++,++start)
	{
		if(i%NMGL_MAX_PALETTE_WIDTH == 0) {start =0;curw<<=2;}
		nmglColorSubTableEXT (NMGL_TEXTURE_2D, start,curw-start, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data);
		 if(!check_palette(cntxt,start,curw-start,data)){TEST_ASSERT(0);}
		inc_palette(data,curw);
	}

	
	
	TEST_ASSERT(1);
return 0;
}
