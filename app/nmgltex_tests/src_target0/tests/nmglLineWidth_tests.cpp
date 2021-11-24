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
void nmglLineWidth (NMGLfloat width);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglLineWidth_wrongArgs_isError();
int nmglLineWidth_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglLineWidth_test()
{
	
	printf ("\nStart nmglLineWidth tests\n\n");	
		RUN_TEST(nmglLineWidth_wrongArgs_isError);
		RUN_TEST(nmglLineWidth_contextCorrect);
	printf ("\nEnd nmglLineWidth tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

int nmglLineWidth_wrongArgs_isError(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;

    nmglLineWidth(-1.0);
    TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
    cntxt->error = NMGL_NO_ERROR;

    nmglLineWidth(0.0);
    TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
    cntxt->error = NMGL_NO_ERROR;

    nmglLineWidth(1.0);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    TEST_ASSERT( cntxt->line.width == 1.0 );
}




int nmglLineWidth_contextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;

    NMGLfloat it = 0.1;
    while( it < 1.1)
    {
        nmglLineWidth(it);
        TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
        TEST_ASSERT( cntxt->line.width == it );
        DEBUG_PRINT(("LineWidth: %x %x \n",cntxt->line.width, it));
        it+=0.1;
    }
    


}

