#include "demo3d_nm0.h"
#include "demo3d_nm1.h"
#include "tests.h"
#include "nmgl_tex_test_common.h"
#include "string.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

#define DEBUG_LEVEL 2
NMGLenum glString_test_array[4] = {NMGL_VENDOR, NMGL_RENDERER, NMGL_VERSION, NMGL_EXTENSIONS };
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const NMGLubyte* nmglGetString(NMGLenum name)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии

int nmglGetString_wrongArgs_isError();
int nmglGetString_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglGetString_test()
{
	
	printf ("\nStart nmglGetString tests\n\n");			
		RUN_TEST(nmglGetString_wrongArgs_isError);
		RUN_TEST(nmglGetString_contextCorrect);
	printf ("\nEnd nmglGetString tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================
int nmglGetString_wrongArgs_isError(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    NMGLubyte const *tstring;
    cntxt->error = NMGL_NO_ERROR;

    tstring = nmglGetString(NMGL_VENDOR-1);
    TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM)
    cntxt->error = NMGL_NO_ERROR;

    for(i=0;i<4;i++){
        tstring = nmglGetString(glString_test_array[i]);
        TEST_ASSERT(cntxt->error == NMGL_NO_ERROR)

    }  
    

    return 0;
}


 

int check(NMGL_Context_NM0 *cntxt, NMGLubyte const *p,NMGLubyte *tstring, NMGLubyte pname ){
        
    int i=0;
    p = nmglGetString(pname);
    for(i=0;i<50;i++)
    {
        if(*(p+i) == '\0') break;
        tstring[i] = *(p+i);
    }
    switch(pname){

        case NMGL_VENDOR:
            TEST_ASSERT(strcmp((char*)p,"RC MODULE") == 0 );
		break;
		
	    case NMGL_RENDERER:
		    TEST_ASSERT(strcmp((char*)p,"Neuromatrix (NMC4)") == 0 );
		break;

	    case NMGL_VERSION:
		    TEST_ASSERT(strcmp((char*)p,"OpenGL SC 1.0") == 0 ); 
		break;
		
	    case NMGL_EXTENSIONS:
		    TEST_ASSERT(strcmp((char*)p,"EXT_paletted_texture EXT_shared_texture_palette") == 0 );
		break;


    }

    TEST_ASSERT(cntxt->error == NMGL_NO_ERROR)
    for(i=0;i<50;i++)
    {       
        tstring[i] = '\0';
    }
    return 0;
}

int nmglGetString_contextCorrect(){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    int i;
    cntxt->error = NMGL_NO_ERROR;
    NMGLubyte tstring[50]={'\0'};
    NMGLubyte const *p;
     
    cntxt->error = NMGL_NO_ERROR;


    i = check(cntxt,p,tstring,NMGL_VENDOR);
    if(i != 0) return -1;
    i = check(cntxt,p,tstring,NMGL_RENDERER);
    if(i != 0) return -1;
    i = check(cntxt,p,tstring,NMGL_VERSION);
    if(i != 0) return -1;
    i = check(cntxt,p,tstring,NMGL_EXTENSIONS);
    if(i != 0) return -1;
    
    
    return 0;
 
}

