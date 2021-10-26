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
void nmglBlendFunc (NMGLenum sfactor, NMGLenum dfactor);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglBlendFunc_wrongArgs_isError();
int nmglBlendFunc_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglBlendFunc_test()
{
	
	printf ("\nStart nmglBlendFunc tests\n\n");	
		RUN_TEST(nmglBlendFunc_wrongArgs_isError);
		RUN_TEST(nmglBlendFunc_contextCorrect);
	printf ("\nEnd nmglBlendFunc tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

int nmglBlendFunc_wrongArgs_isError(){
   NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;
    NMGLenum dfactors[5] = {NMGL_ZERO+1, NMGL_ONE_MINUS_SRC_ALPHA, NMGL_ONE, NMGL_SRC_ALPHA , NMGL_SRC_ALPHA_SATURATE };
   
    for(i=0;i<5;i++)
    {
        nmglBlendFunc(NMGL_ONE,dfactors[i]);
        TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
        cntxt->error = NMGL_NO_ERROR;
    }
    dfactors[0] = NMGL_ZERO;
    dfactors[1]+=1;

    for(i=0;i<5;i++)
    {
        nmglBlendFunc(NMGL_SRC_ALPHA,dfactors[i]);
        TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
        cntxt->error = NMGL_NO_ERROR;
    }
    dfactors[1] = NMGL_ONE_MINUS_SRC_ALPHA;
    dfactors[2]+=1;
    for(i=0;i<5;i++)
    {
        nmglBlendFunc(NMGL_SRC_ALPHA_SATURATE,dfactors[i]);
        TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
        cntxt->error = NMGL_NO_ERROR;
    }
    return 0;
}




int nmglBlendFunc_contextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;
   
    nmglBlendFunc(NMGL_ONE,NMGL_ZERO);  
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    TEST_ASSERT(cntxt->blend.sfactor == NMGL_ONE);
    TEST_ASSERT(cntxt->blend.dfactor == NMGL_ZERO); 
     
    nmglBlendFunc(NMGL_SRC_ALPHA,NMGL_ONE_MINUS_SRC_ALPHA);  
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    TEST_ASSERT(cntxt->blend.sfactor == NMGL_SRC_ALPHA);
    TEST_ASSERT(cntxt->blend.dfactor == NMGL_ONE_MINUS_SRC_ALPHA);

    nmglBlendFunc(NMGL_SRC_ALPHA_SATURATE,NMGL_ONE);  
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    TEST_ASSERT(cntxt->blend.sfactor == NMGL_SRC_ALPHA_SATURATE);
    TEST_ASSERT(cntxt->blend.dfactor == NMGL_ONE);
    return 0;
}

