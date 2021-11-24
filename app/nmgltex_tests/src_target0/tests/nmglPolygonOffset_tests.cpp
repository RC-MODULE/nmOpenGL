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
void nmglPolygonOffset (NMGLfloat factor, NMGLfloat units);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии

int nmglPolygonOffset_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglPolygonOffset_test()
{
	
	printf ("\nStart nmglPolygonOffset tests\n\n");	
		
		RUN_TEST(nmglPolygonOffset_contextCorrect);
	printf ("\nEnd nmglPolygonOffset tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

int nmglPolygonOffset_contextCorrect(){

    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;
        
    nmglPolygonOffset(1.0, 5.0); 
    TEST_ASSERT(cntxt->polygon.offset.factor == 1.0);
    TEST_ASSERT(cntxt->polygon.offset.units  == 5.0);
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);  
 
}

