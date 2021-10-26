#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

#define DEBUG_LEVEL 2

NMGLenum lights[8] = {NMGL_LIGHT0,NMGL_LIGHT1,NMGL_LIGHT2,NMGL_LIGHT3,NMGL_LIGHT4,NMGL_LIGHT5,NMGL_LIGHT6,NMGL_LIGHT7};
NMGLenum pnames_lights[5] = {NMGL_AMBIENT, NMGL_DIFFUSE, NMGL_SPECULAR, NMGL_POSITION, (NMGL_POSITION+1)};
 NMGLfloat light_params[4];
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglGetLightfv (NMGLenum light, NMGLenum pname, NMGLfloat *params);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии

int nmglGetLightfv_contextCorrect();
int nmglGetLightfv_wrongArgs_isError();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglGetLightfv_test()
{
	
	printf ("\nStart nmglGetLightfv tests\n\n");	
		
		RUN_TEST(nmglGetLightfv_wrongArgs_isError);
		RUN_TEST(nmglGetLightfv_contextCorrect);
	printf ("\nEnd nmglGetLightfv tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================

int check_light( NMGL_Context_NM0 *cntxt, NMGLenum light, NMGLenum pname,NMGLfloat* params )
{
    int i;
    TEST_ASSERT( (light == NMGL_LIGHT0) || (light == NMGL_LIGHT1) );
    light-= NMGL_LIGHT0;
    switch(pname){
        
        case NMGL_AMBIENT:
            for(i=0;i<4;i++){
                TEST_ASSERT( cntxt->lightingInfo.lightAmbient[light].vec[i] == *(params+i));
            }
            
        return 0;
        case NMGL_DIFFUSE:

            for(i=0;i<4;i++){
                    TEST_ASSERT( cntxt->lightingInfo.lightDiffuse[light].vec[i] == *(params+i));
                }
        return 0;
        case NMGL_SPECULAR:
            for(i=0;i<4;i++){
                    TEST_ASSERT( cntxt->lightingInfo.lightSpecular[light].vec[i] == *(params+i));
                }
        return 0;
        case NMGL_POSITION:
            for(i=0;i<4;i++){
                    TEST_ASSERT( cntxt->lightingInfo.lightPosition[light].vec[i] == *(params+i));
                }
        return 0;
    }
    return -1;
}



int nmglGetLightfv_wrongArgs_isError(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i,j;
    cntxt->error = NMGL_NO_ERROR;

    for(i=2;i<8;i++)
    {
        for(j=0;j<5;j++)
        {
            nmglGetLightfv(lights[i],pnames_lights[j],light_params);
            TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
            cntxt->error = NMGL_NO_ERROR;
        }
    }
    for(i=0;i<2;i++)
    {
        nmglGetLightfv(lights[i],NMGL_AMBIENT-1,light_params);
        TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
        cntxt->error = NMGL_NO_ERROR;
       
    }
    for(i=0;i<2;i++)
    {
        for(j=0;j<4;j++)
        {
            nmglGetLightfv(lights[i],pnames_lights[j],light_params);
            TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);           
        }
    }
    return 0;
}
int nmglGetLightfv_contextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i,j;
    cntxt->error = NMGL_NO_ERROR;
    
    for(i=0;i<2;i++)
    {
        for(j=0;j<4;j++)
        {
            nmglGetLightfv(lights[i],pnames_lights[j],light_params);
            TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
            if( check_light(cntxt, lights[i],pnames_lights[j],light_params) != 0) return -1; 
                     
        }
    }


 return 0;
 
}

