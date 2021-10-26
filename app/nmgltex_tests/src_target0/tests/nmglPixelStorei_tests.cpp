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
void nmglPixelStorei (NMGLenum pname, NMGLint param);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglPixelStorei_wrongArgs_isError();
int nmglPixelStorei_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglPixelStorei_test()
{	
	printf ("\nStart nmglPixelStorei tests\n\n");	
		RUN_TEST(nmglPixelStorei_wrongArgs_isError);
		RUN_TEST(nmglPixelStorei_contextCorrect);
	printf ("\nEnd nmglPixelStorei tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================




int nmglPixelStorei_wrongArgs_isError()
{
	
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;
    cntxt->unpackAlignment = 4;
    cntxt->packAlignment = 4;

    nmglPixelStorei (NMGL_UNPACK_ALIGNMENT-1, 1);

	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
	TEST_ASSERT(cntxt->unpackAlignment == 4);
	TEST_ASSERT(cntxt->packAlignment == 4);
    cntxt->error = NMGL_NO_ERROR;

    nmglPixelStorei (NMGL_PACK_ALIGNMENT+10, -1);

	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
    TEST_ASSERT(cntxt->unpackAlignment == 4);
    TEST_ASSERT(cntxt->packAlignment == 4);
    cntxt->error = NMGL_NO_ERROR;

    nmglPixelStorei (NMGL_UNPACK_ALIGNMENT+25, 3000);

	TEST_ASSERT(cntxt->error==NMGL_INVALID_ENUM);
	TEST_ASSERT(cntxt->unpackAlignment == 4);
    TEST_ASSERT(cntxt->packAlignment == 4);

    cntxt->error = NMGL_NO_ERROR;

    for(i=0;i<10;i++)    
    {
        if((i == 1)||(i == 2)||(i == 4)||(i == 8)){continue;}
        nmglPixelStorei (NMGL_UNPACK_ALIGNMENT, i);

	    TEST_ASSERT(cntxt->error==NMGL_INVALID_VALUE);
	    TEST_ASSERT(cntxt->unpackAlignment == 4);
        TEST_ASSERT(cntxt->packAlignment == 4);
        cntxt->error = NMGL_NO_ERROR;
    }


	return 0;
}


int nmglPixelStorei_contextCorrect()
{
    
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

    cntxt->error = NMGL_NO_ERROR;
    cntxt->unpackAlignment = 0;
    cntxt->packAlignment = 0;
    int i;
    
    for(i=1;i<9;i++)    
    {
         if((i != 1) && (i != 2) && (i != 4) && (i != 8)) {continue;}
        
        nmglPixelStorei (NMGL_UNPACK_ALIGNMENT, i);
        DEBUG_PRINT2(("pixelStore:%d\n",i));
        DEBUG_PRINT2(("cntxt_error:%d\n",cntxt->error));
        DEBUG_PRINT2(("error0:%d\n",NMGL_NO_ERROR));
        DEBUG_PRINT2(("NMGL_INVALID_VALUE:%d\n",NMGL_INVALID_VALUE));
        DEBUG_PRINT2(("NMGL_INVALID_ENUM:%d\n",NMGL_INVALID_ENUM));
	    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
        TEST_ASSERT(cntxt->unpackAlignment == i);
        TEST_ASSERT(cntxt->packAlignment == 0);
        cntxt->unpackAlignment = 0;

        nmglPixelStorei (NMGL_PACK_ALIGNMENT, i);

	    TEST_ASSERT(cntxt->error==NMGL_NO_ERROR);
        TEST_ASSERT(cntxt->unpackAlignment == 0);
        TEST_ASSERT(cntxt->packAlignment == i);

        cntxt->packAlignment = 0;
    }


    return 0;
}