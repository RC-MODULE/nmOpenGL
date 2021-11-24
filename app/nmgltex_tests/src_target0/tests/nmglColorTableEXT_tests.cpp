#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "nmgl_tex_test_common.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif
extern NMGL_Context_NM1 *cntxt_nm1;
extern unsigned int nmpu1IsAccessible;

#define REF_OBJ_NUMBER 0	
 void* data;


#define DEBUG_LEVEL 1
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglColorTableEXT (NMGLenum target, NMGLenum internalformat, NMGLsizei width, NMGLenum format, NMGLenum type, const void *data);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglColorTableEXT_wrongArgs_isError();
int nmglColorTableEXT_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglColorTableEXT_test()
{
	
	printf ("\nStart nmglColorTableEXT tests\n\n");	
		RUN_TEST(nmglColorTableEXT_wrongArgs_isError);
		RUN_TEST(nmglColorTableEXT_contextCorrect);
	printf ("\nEnd nmglColorTableEXT tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================


// 
int nmglColorTableEXT_wrongArgs_isError()
{
	
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	data=(NMGLubyte*)((NMGLubyte*)cntxt->texState.get_palette_by_name_p(NMGL_MAX_TEX_OBJECTS-1));
	cntxt->error=NMGL_NO_ERROR;
	cntxt->texState.activeTexUnit=NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex=0;
	cntxt->texState.texUnits[0].boundTexObject=&cntxt->texState.texObjects[REF_OBJ_NUMBER];
	
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
cntxt->error=NMGL_NO_ERROR;	
	nmglColorTableEXT (NMGL_SHARED_TEXTURE_PALETTE_EXT, NMGL_RGBA, NMGL_MAX_PALETTE_WIDTH, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data);
	TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
	
	return 0;
}
//------------------------------------------------------------------------------
// 
int nmglColorTableEXT_contextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	int i=0;
	unsigned int cwdth0=0;
	unsigned int cwdth1=0;
	data=(NMGLubyte*)cntxt->texState.get_palette_by_name_p(NMGL_MAX_TEX_OBJECTS-1);
	DEBUG_PRINT1(("NM1 context pointer is 0x%x\n",cntxt_nm1));

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
	DEBUG_PRINT1(("cwidth=%d 0width=%d\n", cwidth, *cntxt->texState.texObjects[0].palette.width));
	TEST_ASSERT(cwidth == *cntxt->texState.texObjects[0].palette.width);
	if(nmpu1IsAccessible == 1) {
		cwdth1=*cntxt_nm1->texState.texObjects[0].palette.width;
		DEBUG_PRINT1(("cwidth=%d 1width=%d\n", cwidth, cwdth1));
		DEBUG_PRINT1(("NM0 pointers:"));
		for(int i=0;i<NMGL_MAX_TEX_OBJECTS;i++)
		{
			DEBUG_PRINT1(("%d %x objAddr=%x\n",i,cntxt->texState.palette_pointers[i+1],cntxt->texState.texObjects[i].palette.colors));
		}
		DEBUG_PRINT1(("NM1 pointers:"));
		for(int i=0;i<NMGL_MAX_TEX_OBJECTS;i++)
		{
			DEBUG_PRINT1(("%d %x objAddr=%x\n",i,cntxt_nm1->texState.palette_pointers[i+1],cntxt_nm1->texState.texObjects[i].palette.colors));
		}
		DEBUG_PRINT1(("NM0 widths:"));
		for(int i=0;i<NMGL_MAX_TEX_OBJECTS;i++)
		{
			DEBUG_PRINT1(("%d %d addr=0x%x ObwAddr=0x%x objw=%d\n",i,*cntxt->texState.paletts_widths_pointers[i+1],cntxt->texState.paletts_widths_pointers[i+1],cntxt->texState.texObjects[i].palette.width,*cntxt->texState.texObjects[i].palette.width));
		}
		DEBUG_PRINT1(("NM1 widths:"));
		for(int i=0;i<NMGL_MAX_TEX_OBJECTS;i++)
		{
			DEBUG_PRINT1(("%d %d addr=0x%x ObwAddr=0x%x objw=%d\n",i,*cntxt_nm1->texState.paletts_widths_pointers[i+1],cntxt_nm1->texState.paletts_widths_pointers[i+1],cntxt_nm1->texState.texObjects[i].palette.width,*cntxt_nm1->texState.texObjects[i].palette.width));
		}
		DEBUG_PRINT1(("Writing to width1 addr:"));
		
		TEST_ASSERT(cwdth1 == cwidth);
	}
	
	for (i=0;i<cwidth*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		if(*((NMGLubyte*)data+i) != *((NMGLubyte*)(cntxt->texState.texObjects[0].palette.colors+i)))
		{
			
			printf("i=%d\ndata[%d]=%x pale[%d]=%d data_nm0[%d]\n",i,i,*((NMGLubyte*)data+i),i,*((NMGLubyte*)(cntxt->texState.texObjects[0].palette.colors+i)),i);
			TEST_ASSERT(0);
		}
		if(nmpu1IsAccessible == 1)
		{
			if(*((NMGLubyte*)data+i) != *((NMGLubyte*)(cntxt_nm1->texState.texObjects[0].palette.colors+i)))
			{
				printf("i=%d\ndata[%d]=%x pale[%d]=%d data_nm1[%d]\n",i,i, *((NMGLubyte*)data + i), i, *((NMGLubyte*)(cntxt_nm1->texState.texObjects[0].palette.colors + i)), i);
				TEST_ASSERT(0);
			}
		}
	}
	TEST_ASSERT(1);
nmglColorTableEXT (NMGL_SHARED_TEXTURE_PALETTE_EXT, NMGL_RGBA, cwidth, NMGL_RGBA, NMGL_UNSIGNED_BYTE, data);
	wait_for_nm1_if_available;
	TEST_ASSERT(cwidth == *cntxt->texState.paletts_widths_pointers[0]);
	if(nmpu1IsAccessible == 1) {
		cwdth1=*cntxt_nm1->texState.paletts_widths_pointers[0];
		DEBUG_PRINT1(("cwidth=%d 1width=%d\n", cwidth, cwdth1));		
		
		TEST_ASSERT(cwdth1 == cwidth);
	}
	for (i=0;i<cwidth*RGBA_TEXEL_SIZE_UBYTE;i++)
	{
		if(*((NMGLubyte*)data+i) != *((NMGLubyte*)(cntxt->texState.palette_pointers[0]+i)))
		{
			
			DEBUG_PRINT1(("i=%d\ndata[%d]=%x pale[%d]=%d data_nm0[%d]\n",i,i,*((NMGLubyte*)data+i),i,*((NMGLubyte*)(cntxt->texState.palette_pointers[0]+i)),i));
			TEST_ASSERT(0);
		}
		if(nmpu1IsAccessible == 1)
		{
			if(*((NMGLubyte*)data+i) != *((NMGLubyte*)(cntxt_nm1->texState.palette_pointers[0]+i)))
			{
				DEBUG_PRINT1(("i=%d\ndata[%d]=%x pale[%d]=%d data_nm1[%d]\n",i,i, *((NMGLubyte*)data + i), i, *((NMGLubyte*)(cntxt_nm1->texState.palette_pointers[0] + i)), i));
				TEST_ASSERT(0);
			}
		}
	}
return 0;
}
