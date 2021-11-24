#include "demo3d_nm0.h"
#include "tests.h"

#ifdef __GNUC__
#pragma code_section ".text_tex_tests"
#pragma data_section ".data_tex_tests"
#endif

NMGLenum pnames_booleanv[25] = {NMGL_SHARED_TEXTURE_PALETTE_EXT,NMGL_COLOR_WRITEMASK,NMGL_DEPTH_WRITEMASK,
NMGL_VERTEX_ARRAY,NMGL_NORMAL_ARRAY,NMGL_COLOR_ARRAY,NMGL_TEXTURE_COORD_ARRAY,
NMGL_NORMALIZE,NMGL_RESCALE_NORMAL,NMGL_LIGHTING,NMGL_COLOR_MATERIAL,NMGL_LIGHT0,
NMGL_LIGHT1,NMGL_POINT_SMOOTH,NMGL_LINE_SMOOTH,NMGL_LINE_STIPPLE,NMGL_CULL_FACE,
NMGL_POLYGON_OFFSET_FILL,NMGL_POLYGON_STIPPLE,NMGL_TEXTURE_2D,NMGL_SCISSOR_TEST,
NMGL_ALPHA_TEST,NMGL_STENCIL_TEST,NMGL_DEPTH_TEST,NMGL_BLEND};
NMGLboolean params_boolean[4];
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nmglGetBooleanv (NMGLenum pname, NMGLboolean *params);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
//Тестовые сценарии
int nmglGetBooleanv_wrongArgs_isError();
int nmglGetBooleanv_contextCorrect();
//----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//===========================MAIN=========================================================
//========================================================================================
//Главная  функция
int run_nmglGetBooleanv_test()
{
	
	
	printf ("\nStart nmglGetBooleanv tests\n\n");
		RUN_TEST(nmglGetBooleanv_wrongArgs_isError);
		RUN_TEST(nmglGetBooleanv_contextCorrect);
	printf ("\nEnd nmglGetBooleanv tests\n");

	return 0;
}

//========================================================================================
//========================================================================================
//========================================================================================


// 
int check_booleanv(NMGL_Context_NM0 *cntxt, NMGLenum pname,NMGLboolean *output){
	switch(pname){
	case NMGL_SHARED_TEXTURE_PALETTE_EXT:
        return output[0] == cntxt->texState.shared_palette_enabled;
    break;

    case NMGL_COLOR_WRITEMASK:
        int i;
        for(i=0;i<4;i++){
            if(output[i] != cntxt->color_write_mask_flags[i]) return 0;
        }
        return 1;
    break;

    case NMGL_DEPTH_WRITEMASK:
        return output[0] == cntxt->depth_write_mask_enabled;
    break;

    case NMGL_VERTEX_ARRAY:
        return output[0] == cntxt->vertexArray.enabled;
    break;

    case NMGL_NORMAL_ARRAY:
        return output[0] == cntxt->normalArray.enabled;
    break;

    case NMGL_COLOR_ARRAY:
        return output[0] == cntxt->colorArray.enabled;
    break;

    case NMGL_TEXTURE_COORD_ARRAY:
        return output[0] == cntxt->texState.texcoordArray[cntxt->texState.activeTexUnit].enabled;
    break;

    case NMGL_NORMALIZE:
        return output[0] == cntxt->normalizeEnabled;
    break;

    case NMGL_RESCALE_NORMAL:
        return output[0] == NMGL_FALSE;
    break;
    
    case NMGL_LIGHTING:
        return output[0] == cntxt->lightingInfo.isLighting;
    break;

    case NMGL_COLOR_MATERIAL:
        return output[0] == cntxt->lightingInfo.isColorMaterial;
    break;

    case NMGL_LIGHT0:
	case NMGL_LIGHT1:
	case NMGL_LIGHT2:
	case NMGL_LIGHT3:
	case NMGL_LIGHT4:
	case NMGL_LIGHT5:
	case NMGL_LIGHT6:
	case NMGL_LIGHT7:
        return output[0] == cntxt->lightingInfo.isEnabledLight[pname - NMGL_LIGHT0];
    break;

    case NMGL_POINT_SMOOTH:
		return output[0] == cntxt->point.smooth_enabled;
	break;

    case NMGL_LINE_SMOOTH:
		return output[0] == cntxt->line.smooth_enabled;
	break;

    case NMGL_LINE_STIPPLE:
		return output[0] == cntxt->line.stipple.enabled;
	break;

    case NMGL_CULL_FACE:
		return output[0] == cntxt->isCullFace;
	break;

    case NMGL_POLYGON_OFFSET_FILL:
		return output[0] == cntxt->polygon.offset_fill_enabled;
	break;

    case NMGL_POLYGON_STIPPLE:
		return output[0] == cntxt->polygon.stipple.enabled;
	break;

    case NMGL_TEXTURE_2D:
		return output[0] == cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled;
	break;

    case NMGL_SCISSOR_TEST:
		return output[0] == cntxt->scissorTest.isEnabled;

    case NMGL_ALPHA_TEST:
		return output[0] == cntxt->alpha_test.enabled;

    case NMGL_STENCIL_TEST:
        return output[0] == cntxt->stencil_test.enabled;

    case NMGL_DEPTH_TEST:
        return output[0] == cntxt->depth_test.enabled;//TODO:add context field

    case NMGL_BLEND:
        return output[0] == cntxt->blend.enabled ;

   
    }
    return 0;
}


int nmglGetBooleanv_wrongArgs_isError()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();	
	cntxt->error = NMGL_NO_ERROR;
    int i;
    nmglGetBooleanv( NMGL_SHARED_TEXTURE_PALETTE_EXT-1,params_boolean);
	TEST_ASSERT(cntxt->error == NMGL_INVALID_ENUM);
    cntxt->error = NMGL_NO_ERROR;
    for(i=0;i<25;i++){
        nmglGetBooleanv( pnames_booleanv[i],params_boolean);
        TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
    }


	return 0;
}
//------------------------------------------------------------------------------
// 
int nmglGetBooleanv_contextCorrect()
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();	
    int i;
	cntxt->error=NMGL_NO_ERROR;
    for(i=0;i<25;i++){
        nmglGetBooleanv( pnames_booleanv[i],params_boolean);
        TEST_ASSERT(cntxt->error == NMGL_NO_ERROR);
        TEST_ASSERT(check_booleanv(cntxt,pnames_booleanv[i],params_boolean));

    }

return 0;
}



	