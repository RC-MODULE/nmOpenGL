#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

#define DEBUG_LEVEL 2
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglLineStipple (NMGLint factor, NMGLushort pattern);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglLineStipple_wrongArgs_isError();
int nmglLineStipple_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglLineStipple_test()
{
	
	//std::cout << "Hello";
	printf ("\nStart nmglLineStipple tests\n\n");	
		RUN_TEST(nmglLineStipple_wrongArgs_isError);
		RUN_TEST(nmglLineStipple_contextCorrect);
	printf ("\nEnd nmglLineStipple tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

int nmglLineStipple_wrongArgs_isError(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;

    cntxt->line.stipple.enabled = NMGL_FALSE;

    nmglLineStipple(-1,1);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);
    cntxt->error = NMGL_NO_ERROR;

    nmglLineStipple(0,1);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);
    cntxt->error = NMGL_NO_ERROR;

    nmglLineStipple(1,1);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

    cntxt->line.stipple.enabled = NMGL_TRUE;

    nmglLineStipple(-1,1);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);
    cntxt->error = NMGL_NO_ERROR;

    nmglLineStipple(0,1);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_VALUE);
    cntxt->error = NMGL_NO_ERROR;

    nmglLineStipple(1,1);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

    cntxt->line.stipple.enabled = NMGL_FALSE;
    return 0;
}




int nmglLineStipple_contextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;
    cntxt->line.stipple.enabled = NMGL_FALSE;

    for(i=1;i<257;i++)
    {
        nmglLineStipple(i,1);
        TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
        TEST_ASSERT(cntxt->line.stipple.factor == i);
        TEST_ASSERT(cntxt->line.stipple.pattern == 0xFFFF);
    }
    
    cntxt->line.stipple.enabled = NMGL_TRUE;

    nmglLineStipple(35,55786);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    TEST_ASSERT(cntxt->line.stipple.factor == 35);
    TEST_ASSERT(cntxt->line.stipple.pattern == 55786 & 0xFFFF);

    nmglLineStipple(359,55786);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    TEST_ASSERT(cntxt->line.stipple.factor == 103);
    TEST_ASSERT(cntxt->line.stipple.pattern == 55786 & 0xFFFF);

    nmglLineStipple(35,1921623);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    TEST_ASSERT(cntxt->line.stipple.factor == 35);
    TEST_ASSERT(cntxt->line.stipple.pattern == 21079);

    nmglLineStipple(359,1921623);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    TEST_ASSERT(cntxt->line.stipple.factor == 103);
    TEST_ASSERT(cntxt->line.stipple.pattern == 21079);

    cntxt->line.stipple.enabled = NMGL_FALSE;
    return 0;
}

