#include "demo3d_nm0.h"
#include "tests.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

NMGLenum pnames_isEnabled[23] = {NMGL_SHARED_TEXTURE_PALETTE_EXT,
 NMGL_VERTEX_ARRAY, NMGL_COLOR_ARRAY, NMGL_NORMAL_ARRAY, NMGL_TEXTURE_COORD_ARRAY,
 NMGL_NORMALIZE, NMGL_RESCALE_NORMAL, NMGL_LIGHTING, NMGL_COLOR_MATERIAL, NMGL_LIGHT0,
 NMGL_LIGHT1, NMGL_POINT_SMOOTH, NMGL_LINE_SMOOTH, NMGL_LINE_STIPPLE, NMGL_CULL_FACE,
 NMGL_POLYGON_OFFSET_FILL, NMGL_POLYGON_STIPPLE, NMGL_TEXTURE_2D, NMGL_SCISSOR_TEST,
 NMGL_ALPHA_TEST, NMGL_STENCIL_TEST, NMGL_DEPTH_TEST, NMGL_BLEND };
NMGLboolean isEnabled_output;
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NMGLboolean nmglIsEnabled (NMGLenum cap);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglIsEnabled_wrongArgs_isError();
int nmglIsEnabled_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglIsEnabled_test()
{
	
	
	printf ("\nStart nmglIsEnabled tests\n\n");
		RUN_TEST(nmglIsEnabled_wrongArgs_isError);
		RUN_TEST(nmglIsEnabled_contextCorrect);
	printf ("\nEnd nmglIsEnabled tests\n");

	    return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================
int check_isEnabled(NMGL_Context_NM0 *cntxt, NMGLenum pname,NMGLboolean output){
    switch(pname){
    	DEBUG_PRINT(("output=%i\n",output));
    case NMGL_SHARED_TEXTURE_PALETTE_EXT:
		DEBUG_PRINT(("NMGL_SHARED_TEXTURE_PALETTE_EXT\n"));
    return (output == cntxt->texState.shared_palette_enabled);

    case NMGL_VERTEX_ARRAY:
        DEBUG_PRINT(("NMGL_VERTEX_ARRAY\n"));

    return (output == cntxt->vertexArray.enabled);

    case NMGL_COLOR_ARRAY:
        DEBUG_PRINT(("NMGL_COLOR_ARRAY\n"));
    return (output == cntxt->colorArray.enabled);

    case NMGL_NORMAL_ARRAY:
        DEBUG_PRINT(("NMGL_NORMAL_ARRAY\n"));
    return (output == cntxt->normalArray.enabled);
        
    case NMGL_TEXTURE_COORD_ARRAY:
        DEBUG_PRINT(("NMGL_TEXTURE_COORD_ARRAY\n"));
    return (output == cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnitIndex].enabled);
        
    case NMGL_NORMALIZE:
        DEBUG_PRINT(("NMGL_NORMALIZE\n"));
    return (output == (NMGLboolean) cntxt->normalizeEnabled);
    
    case NMGL_RESCALE_NORMAL:
        DEBUG_PRINT(("NMGL_RESCALE_NORMAL\n"));
    return (output == NMGL_FALSE);
    
    case NMGL_LIGHTING:
        DEBUG_PRINT(("NMGL_LIGHTING\n"));
    return (output == (NMGLboolean)cntxt->lightingInfo.isLighting);

    case NMGL_COLOR_MATERIAL:
        DEBUG_PRINT(("NMGL_LIGHTING\n"));
    return (output == (NMGLboolean) cntxt->lightingInfo.isColorMaterial);

    case NMGL_LIGHT0:
	case NMGL_LIGHT1:	
	    DEBUG_PRINT(("NMGL_LIGHT\n"));
    return (output == (NMGLboolean) cntxt->lightingInfo.isEnabledLight[pname - NMGL_LIGHT0]);

    case NMGL_POINT_SMOOTH:    
        DEBUG_PRINT(("NMGL_POINT_SMOOTH\n"));
    return (output == cntxt->point.smooth_enabled);

    case NMGL_LINE_SMOOTH:    
        DEBUG_PRINT(("NMGL_LINE_SMOOTH\n"));
    return (output == cntxt->line.smooth_enabled);

    case NMGL_LINE_STIPPLE:    
        DEBUG_PRINT(("NMGL_LINE_STIPPLE\n"));
    return (output == cntxt->line.stipple.enabled);

    case NMGL_CULL_FACE:
       DEBUG_PRINT(("NMGL_CULL_FACE\n"));
    return (output == (NMGLboolean) cntxt->isCullFace);
    
    case NMGL_POLYGON_OFFSET_FILL:
       DEBUG_PRINT(("NMGL_POLYGON_OFFSET_FILL\n"));
    return (output == cntxt->polygon.offset_fill_enabled);

    case NMGL_POLYGON_STIPPLE:
       DEBUG_PRINT(("NMGL_POLYGON_STIPPLE\n"));
    return (output == cntxt->polygon.stipple.enabled);

    case NMGL_TEXTURE_2D:
        DEBUG_PRINT(("NMGL_TEXTURE_2D\n"));
    return (output == cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled);

    case NMGL_SCISSOR_TEST:
		DEBUG_PRINT(("NMGL_SCISSOR_TEST\n"));
    return (output == (NMGLboolean) cntxt->scissorTest.isEnabled);

    case NMGL_ALPHA_TEST:
		DEBUG_PRINT(("NMGL_ALPHA_TEST\n"));
    return (output == cntxt->alpha_test.enabled);

    case NMGL_STENCIL_TEST:
        DEBUG_PRINT(("NMGL_STENCIL_TEST\n"));
    return (output == cntxt->stencil_test.enabled);

    case NMGL_DEPTH_TEST:
        DEBUG_PRINT(("NMGL_DEPTH_TEST\n"));
    return (output == cntxt->depth_test.enabled);//TODO:add context field

    case NMGL_BLEND:
        DEBUG_PRINT(("NMGL_BLEND\n"));
    return (output == cntxt->blend.enabled);



    }
}

// 
int nmglIsEnabled_wrongArgs_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();		
	cntxt->error=NMGL_NO_ERROR;

    int i=0;	

    isEnabled_output = nmglIsEnabled(NMGL_SHARED_TEXTURE_PALETTE_EXT+1);
    TEST_ASSERT(cntxt->error = NMGL_INVALID_ENUM);
    cntxt->error=NMGL_NO_ERROR;

    for(i=0;i<22;i++){
        isEnabled_output = nmglIsEnabled(pnames_isEnabled[i]);
        TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    }


	DEBUG_PRINT(("\n"));
    return 0;
}
//------------------------------------------------------------------------------
// 
int nmglIsEnabled_contextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();	
	cntxt->error=NMGL_NO_ERROR;
    int i;
     for(i=0;i<23;i++){
       isEnabled_output = nmglIsEnabled(pnames_isEnabled[i]);
        TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
        TEST_ASSERT(check_isEnabled(cntxt,pnames_isEnabled[i], isEnabled_output));
    }

DEBUG_PRINT(("\n"));
    return 0;
}
