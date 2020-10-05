#include "demo3d_nm0.h"
#include "tests.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglGetColorTableParameterivEXT (NMGLenum target, NMGLenum pname, NMGLint *params);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglGetColorTableParameterivEXT_wrongArgs_isError();
int nmglGetColorTableParameterivEXT_setColorTable_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglGetColorTableParameterivEXT_test()
{
	
	printf ("\nStart nmglGetColorTableParameterivEXT tests\n\n");	
		RUN_TEST(nmglGetColorTableParameterivEXT_wrongArgs_isError);
		RUN_TEST(nmglGetColorTableParameterivEXT_setColorTable_contextCorrect);
	printf ("\nEnd nmglGetColorTableParameterivEXT tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================


// 
int nmglGetColorTableParameterivEXT_wrongArgs_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGLint array;
	cntxt->error=NMGL_NO_ERROR;

	nmglGetColorTableParameterivEXT (NMGL_TEXTURE_2D+1, NMGL_COLOR_TABLE_FORMAT_EXT, &array);
	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
	cntxt->error=NMGL_NO_ERROR;
	return 0;
}
//------------------------------------------------------------------------------
// 
int nmglGetColorTableParameterivEXT_setColorTable_contextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->error=NMGL_NO_ERROR;
	NMGLint array;
	nmglGetColorTableParameterivEXT (NMGL_TEXTURE_2D, NMGL_COLOR_TABLE_FORMAT_EXT, &array);
TEST_ASSERT(array==NMGL_COLOR_INDEX8_EXT);
	ActiveTexObjectP->palette.width=NMGL_MAX_PALETTE_WIDTH;
	nmglGetColorTableParameterivEXT (NMGL_TEXTURE_2D, NMGL_COLOR_TABLE_WIDTH_EXT, &array);
TEST_ASSERT(array==ActiveTexObjectP->palette.width);
	nmglGetColorTableParameterivEXT (NMGL_TEXTURE_2D, NMGL_COLOR_TABLE_RED_SIZE_EXT, &array);
TEST_ASSERT(array==1);
array=0;
	nmglGetColorTableParameterivEXT (NMGL_TEXTURE_2D, NMGL_COLOR_TABLE_GREEN_SIZE_EXT, &array);
	TEST_ASSERT(array==1);
array=0;
	nmglGetColorTableParameterivEXT (NMGL_TEXTURE_2D, NMGL_COLOR_TABLE_BLUE_SIZE_EXT, &array);
	TEST_ASSERT(array==1);
array=0;
	nmglGetColorTableParameterivEXT (NMGL_TEXTURE_2D, NMGL_COLOR_TABLE_ALPHA_SIZE_EXT, &array);
	TEST_ASSERT(array==1);
array=0;
	nmglGetColorTableParameterivEXT (NMGL_TEXTURE_2D, NMGL_COLOR_TABLE_LUMINANCE_SIZE_EXT, &array);
	TEST_ASSERT(array==1);
array=0;
	nmglGetColorTableParameterivEXT (NMGL_TEXTURE_2D, NMGL_COLOR_TABLE_INTENSITY_SIZE_EXT, &array);
	TEST_ASSERT(array==1);


return 0;
}
