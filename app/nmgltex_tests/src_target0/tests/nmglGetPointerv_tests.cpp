#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

#define DEBUG_LEVEL 2
NMGLenum pnames[4] = {NMGL_VERTEX_ARRAY_POINTER, NMGL_NORMAL_ARRAY_POINTER, NMGL_COLOR_ARRAY_POINTER, NMGL_TEXTURE_COORD_ARRAY_POINTER };
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglGetPointerv (NMGLenum pname, NMGLvoid **params);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglGetPointerv_wrongArgs_isError();
int nmglGetPointerv_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglGetPointerv_test()
{
	
	printf ("\nStart nmglGetPointerv tests\n\n");	
		RUN_TEST(nmglGetPointerv_wrongArgs_isError);
		RUN_TEST(nmglGetPointerv_contextCorrect);
	printf ("\nEnd nmglGetPointerv tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

int nmglGetPointerv_wrongArgs_isError(){
   NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
   

    cntxt->error = NMGL_NO_ERROR;
    NMGLvoid * params;
    nmglGetPointerv(NMGL_VERTEX_ARRAY_POINTER+5,&params);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
    cntxt->error = NMGL_NO_ERROR;
    for(i=0;i<4;i++)
    {
        nmglGetPointerv(pnames[i],&params);
        TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    }
}

int nmglGetPointerv_contextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    NMGLvoid * params;
    cntxt->error = NMGL_NO_ERROR;
   for(i=0;i<4;i++)
    {
        nmglGetPointerv(pnames[i],&params);
        TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
        switch(pnames[i]){
            case NMGL_VERTEX_ARRAY_POINTER:
                 TEST_ASSERT(cntxt->vertexArray.pointer == params);
                break;
            case NMGL_NORMAL_ARRAY_POINTER:
                TEST_ASSERT(cntxt->normalArray.pointer == params);
                break;
            case NMGL_COLOR_ARRAY_POINTER:
                TEST_ASSERT(cntxt->colorArray.pointer == params);
                break;
            case NMGL_TEXTURE_COORD_ARRAY_POINTER:
                TEST_ASSERT(cntxt->texState.texcoordArray[cntxt->texState.activeTexUnitIndex].pointer == params);
                break;
        }

    }
}

