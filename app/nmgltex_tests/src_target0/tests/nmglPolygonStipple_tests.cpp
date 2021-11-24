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
void nmglPolygonStipple (const NMGLubyte *mask);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglPolygonStipple_wrongArgs_isError();
int nmglPolygonStipple_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglPolygonStipple_test()
{
	
	printf ("\nStart nmglPolygonStipple tests\n\n");	
		
		RUN_TEST(nmglPolygonStipple_contextCorrect);
	printf ("\nEnd nmglPolygonStipple tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

int nmglPolygonStipple_contextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;
    NMGLubyte test_array[(NMGL_POLIGON_STIPPLE_SIDE_UBYTES)*(NMGL_POLIGON_STIPPLE_SIDE_UBYTES>>3)];
    cntxt->polygon.stipple.enabled = NMGL_FALSE;
    NMGLint iter = (NMGL_POLIGON_STIPPLE_SIDE_UBYTES)*(NMGL_POLIGON_STIPPLE_SIDE_UBYTES>>3);
    for(i=0;i<iter;i++)
    {
        test_array[i] = 0x1+i;
    }
    
    nmglPolygonStipple(test_array);
     for(i=0;i<iter;i++)
    {
        TEST_ASSERT(    *(cntxt->polygon.stipple.pattern+i) == test_array[i] );
    }
    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);

    cntxt->polygon.stipple.enabled = NMGL_TRUE;
    for(i=0;i<iter;i++)
    {
        test_array[i] = 0x1+i*2;
    }
    
    nmglPolygonStipple(test_array);
    for(i=0;i<iter;i++)
    {
        TEST_ASSERT(    *(cntxt->polygon.stipple.pattern+i) == test_array[i] );
    }    
TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
 cntxt->polygon.stipple.enabled = NMGL_FALSE;
 
}

