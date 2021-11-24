#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"


#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglColorMask (NMGLboolean red, NMGLboolean green, NMGLboolean blue, NMGLboolean alpha);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
//int nmglColorMask_wrongArgs_isError();
int nmglColorMask_setRed_setContextCorrect();
int nmglColorMask_setGreen_setContextCorrect();
int nmglColorMask_setBlue_setContextCorrect();
int nmglColorMask_setAlpha_setContextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglColorMask_test()
{
	
	printf ("\nStart nmglColorMask tests\n\n");	
		RUN_TEST(nmglColorMask_setRed_setContextCorrect);
		RUN_TEST(nmglColorMask_setGreen_setContextCorrect);
		RUN_TEST(nmglColorMask_setBlue_setContextCorrect);
		RUN_TEST(nmglColorMask_setAlpha_setContextCorrect);
	printf ("\nEnd nmglColorMask tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

int nmglColorMask_setRed_setContextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    cntxt->color_write_mask_flags[0] = NMGL_FALSE;
    cntxt->color_write_mask_flags[1] = NMGL_TRUE;
    cntxt->color_write_mask_flags[2] = NMGL_TRUE;
    cntxt->color_write_mask_flags[3] = NMGL_TRUE;

    nmglColorMask(NMGL_TRUE,NMGL_FALSE,NMGL_FALSE,NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[0] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[1] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[2] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[3] == NMGL_FALSE);
    
    nmglColorMask(NMGL_FALSE,NMGL_TRUE,NMGL_TRUE,NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[0] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[1] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[2] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[3] == NMGL_TRUE);
    return 0;
}
int nmglColorMask_setGreen_setContextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();    
    cntxt->color_write_mask_flags[0] = NMGL_TRUE;
    cntxt->color_write_mask_flags[1] = NMGL_FALSE;
    cntxt->color_write_mask_flags[2] = NMGL_TRUE;
    cntxt->color_write_mask_flags[3] = NMGL_TRUE;

    nmglColorMask(NMGL_FALSE,NMGL_TRUE,NMGL_FALSE,NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[0] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[1] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[2] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[3] == NMGL_FALSE);
    
    nmglColorMask(NMGL_TRUE,NMGL_FALSE,NMGL_TRUE,NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[0] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[1] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[2] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[3] == NMGL_TRUE);
    return 0;

}
int nmglColorMask_setBlue_setContextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();    
    cntxt->color_write_mask_flags[0] = NMGL_TRUE;
    cntxt->color_write_mask_flags[1] = NMGL_TRUE;
    cntxt->color_write_mask_flags[2] = NMGL_FALSE;
    cntxt->color_write_mask_flags[3] = NMGL_TRUE;

    nmglColorMask(NMGL_FALSE,NMGL_FALSE,NMGL_TRUE,NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[0] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[1] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[2] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[3] == NMGL_FALSE);
    
    nmglColorMask(NMGL_TRUE,NMGL_TRUE,NMGL_FALSE,NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[0] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[1] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[2] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[3] == NMGL_TRUE);
    return 0;

}
int nmglColorMask_setAlpha_setContextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    cntxt->color_write_mask_flags[0] = NMGL_TRUE;
    cntxt->color_write_mask_flags[1] = NMGL_TRUE;
    cntxt->color_write_mask_flags[2] = NMGL_TRUE;
    cntxt->color_write_mask_flags[3] = NMGL_FALSE;

    nmglColorMask(NMGL_FALSE,NMGL_FALSE,NMGL_FALSE,NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[0] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[1] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[2] == NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[3] == NMGL_TRUE);
    
    nmglColorMask(NMGL_TRUE,NMGL_TRUE,NMGL_TRUE,NMGL_FALSE);
    TEST_ASSERT(cntxt->color_write_mask_flags[0] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[1] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[2] == NMGL_TRUE);
    TEST_ASSERT(cntxt->color_write_mask_flags[3] == NMGL_FALSE);
    return 0;
}