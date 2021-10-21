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
void nmglColorSubTableEXT (NMGLenum target, NMGLsizei start, NMGLsizei count, NMGLenum format, NMGLenum type, const void *data1);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
*/
//extern NMGLubyte mem_palette [NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*NMGL_MAX_TEX_OBJECTS];
//void *data1=&mem_palette[(int)(NMGL_MAX_PALETTE_WIDTH*RGBA_TEXEL_SIZE_UBYTE*(NMGL_MAX_TEX_OBJECTS-1))];   
void* data1;
//////////////////////////////////////////////////////////////////////////////////////////////
int check_palette(NMGL_Context_NM0 *cntxt,NMGLsizei start,NMGLsizei count,const void* data1);
void inc_palette(void* data1,NMGLsizei curw);
///
//Тестовые сценарии
int nmglColorSubTableEXT_wrongArgs_isError();
int nmglColorSubTableEXT_setContext_ContextCorrect();
//----------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////////////////////////

int check_palette(NMGLubyte* palette,NMGLsizei start,NMGLsizei count,const void* data1)
{
	int i=0;
	for (i=0;i<count*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		
		if(*((NMGLubyte*)palette + start*RGBA_TEXEL_SIZE_UBYTE+i) != *((NMGLubyte*)data1+i)) return 0;
	}
return 1;//match
}
void inc_palette(void* data1,NMGLsizei curw)
{
	int i=0;
	for(i=0;i<curw*RGBA_TEXEL_SIZE_UBYTE;i++)
	{	
		*((NMGLubyte*)data1+i)+=1;
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
	

	data1=(NMGLubyte*)cntxt->texState.get_palette_by_name_p(NMGL_MAX_TEX_OBJECTS-1);
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];

nmglColorSubTableEXT (NMGL_TEXTURE_2D+1, 0, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE ,data1);
TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, -1, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE ,data1);
TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, NMGL_MAX_PALETTE_WIDTH, 0, NMGL_RGBA,NMGL_UNSIGNED_BYTE ,data1);
TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, NMGL_MAX_PALETTE_WIDTH, 1, NMGL_RGBA,NMGL_UNSIGNED_BYTE ,data1);
TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, 0, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA+1,NMGL_UNSIGNED_BYTE ,data1);
TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
cntxt->error=NMGL_NO_ERROR;

nmglColorSubTableEXT (NMGL_TEXTURE_2D, 0, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE+1 ,data1);
TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
cntxt->error=NMGL_NO_ERROR;
nmglColorSubTableEXT (NMGL_TEXTURE_2D, 0, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE,data1);
TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
nmglColorSubTableEXT (NMGL_TEXTURE_2D, 1, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE,data1);
TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
nmglColorSubTableEXT (NMGL_SHARED_TEXTURE_PALETTE_EXT, 0, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE,data1);
TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
nmglColorSubTableEXT (NMGL_SHARED_TEXTURE_PALETTE_EXT, 1, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA,NMGL_UNSIGNED_BYTE,data1);
TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	return 0;
}
//------------------------------------------------------------------------------
// 
int nmglColorSubTableEXT_setContext_ContextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
data1=(NMGLubyte*)cntxt->texState.get_palette_by_name_p(NMGL_MAX_TEX_OBJECTS-1);
	NMGLint n_bytes=0; 
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[0];
	NMGLsizei  start=0,i;
	NMGLsizei curw=(NMGL_MAX_PALETTE_WIDTH)>>2;
	unsigned int psize=  NMGL_MAX_PALETTE_WIDTH;
	n_bytes = curw;

	//ActiveTexObjectP->palette.setWidth_p(&psize);
	*ActiveTexObjectP->palette.width = psize;
	*cntxt->texState.paletts_widths_pointers[0] = psize;
	//ActiveTexObjectP->palette.setColors((NMGLubyte*)(cntxt->texState.get_palette_by_name_p(ActiveTexObjectP->name)));//delete
	ActiveTexObjectP->palette.colors = ((NMGLubyte*)(cntxt->texState.get_palette_by_name_p(ActiveTexObjectP->name)));//delete
	for(i=0;i<NMGL_MAX_PALETTE_WIDTH;i++)
	{
		*((NMGLubyte*)ActiveTexObjectP->palette.getColors()+i)=0x1+i;
		*((NMGLubyte*)cntxt->texState.palette_pointers[0]+i)=0x2+i;
		*((NMGLubyte*)data1+i)=0x0;
	}
	while(curw <= psize)
	{
		for(start=0;start<NMGL_MAX_PALETTE_WIDTH;++start)
		{
		
			if( start+curw > psize ) n_bytes = psize-start;
			else {n_bytes = curw;} 
			
			nmglColorSubTableEXT (NMGL_TEXTURE_2D, start,n_bytes, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data1);
			nmglColorSubTableEXT (NMGL_SHARED_TEXTURE_PALETTE_EXT, start,n_bytes, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data1);
			 if(!check_palette(cntxt->texState.palette_pointers[ActiveTexObjectP->name+1],start,n_bytes,data1)){TEST_ASSERT(0);}
			 if(!check_palette(cntxt->texState.palette_pointers[0],start,n_bytes,data1)){TEST_ASSERT(0);}
			inc_palette(data1,curw);
			
		}
		curw<<=1;
	}
	

	
	
	TEST_ASSERT(1);
return 0;
}
