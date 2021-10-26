#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"
#include "string.h"



#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif
#define DEBUG
//#define DEBUG_LEVEL 2
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglAlphaFunc (NMGLenum func, NMGLclampf ref);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии

int nmglAlphaFunc_wrongFunc_isError();
int nmglAlphaFunc_setFunc_setContextCorrect();
int nmglAlphaFunc_setRef_setContextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglAlphaFunc_test()
{
	
	printf ("\nStart nmglAlphaFunc tests\n\n");			
		RUN_TEST(nmglAlphaFunc_wrongFunc_isError);
		RUN_TEST(nmglAlphaFunc_setFunc_setContextCorrect);
		RUN_TEST(nmglAlphaFunc_setRef_setContextCorrect);
	printf ("\nEnd nmglAlphaFunc tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================
int nmglAlphaFunc_wrongFunc_isError(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;

    nmglAlphaFunc(NMGL_LEQUAL, 0.5);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    nmglAlphaFunc(NMGL_ALWAYS, 0.5);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

    nmglAlphaFunc(NMGL_ALWAYS+NMGL_LEQUAL, 0.5);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
    
    cntxt->error = NMGL_NO_ERROR;

    return 0;
}

int nmglAlphaFunc_setFunc_setContextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;
    cntxt->alpha_test.func = NMGL_ALWAYS;
    
    nmglAlphaFunc(NMGL_LEQUAL, 0.5);
    TEST_ASSERT( cntxt->alpha_test.func == NMGL_LEQUAL);
    nmglAlphaFunc(NMGL_ALWAYS, 0.5);
    TEST_ASSERT( cntxt->alpha_test.func == NMGL_ALWAYS);
    return 0;
}

int nmglAlphaFunc_setRef_setContextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;
    cntxt->alpha_test.ref = 0.0;

    nmglAlphaFunc(NMGL_ALWAYS, -0.1);
    TEST_ASSERT(equalf(cntxt->alpha_test.ref, 0.0));
    nmglAlphaFunc(NMGL_ALWAYS, 1.5);
    TEST_ASSERT(equalf(cntxt->alpha_test.ref, 1.0));
    nmglAlphaFunc(NMGL_ALWAYS, 0.9);
    DEBUG_PRINT(("cntxt->alpha_test.ref = %.2f",cntxt->alpha_test.ref));
    TEST_ASSERT(equalf(cntxt->alpha_test.ref, 0.9));
    
    nmglAlphaFunc(NMGL_ALWAYS, 0.1);
    TEST_ASSERT(equalf(cntxt->alpha_test.ref, 0.1));
    nmglAlphaFunc(NMGL_ALWAYS, 0.0);
    TEST_ASSERT(equalf(cntxt->alpha_test.ref, 0.0));
   return 0;
 
}

