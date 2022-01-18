#include "demo3d_nm0.h"
#include "nmgl.h"


void nmglGetBooleanv (NMGLenum pname, NMGLboolean *params){

    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    NMGL_Context *context = NMGL_GetCurrentContext();

    switch(pname){
	case NMGL_SHARED_TEXTURE_PALETTE_EXT:
        *params = cntxt->texState.shared_palette_enabled;
    break;

    case NMGL_COLOR_WRITEMASK:
        int i;
        for(i=0;i<4;i++){
            params[i] = context->color_write_mask_flags[i];
        }
    break;

    case NMGL_DEPTH_WRITEMASK:
        *params = context->depth_test.mask;
    break;

    case NMGL_VERTEX_ARRAY:
        *params = context->vertexArray.enabled;
    break;

    case NMGL_NORMAL_ARRAY:
        *params = context->normalArray.enabled;
    break;

    case NMGL_COLOR_ARRAY:
        *params = context->colorArray.enabled;
    break;

    case NMGL_TEXTURE_COORD_ARRAY:
        *params = cntxt->texState.texcoordArray[cntxt->texState.activeTexUnit].enabled;
    break;

    case NMGL_NORMALIZE:
        *params = context->normalizeEnabled;
    break;

    case NMGL_RESCALE_NORMAL:
        *params = NMGL_FALSE;
    break;
    
    case NMGL_LIGHTING:
        *params = context->lightingInfo.isLighting;
    break;

    case NMGL_COLOR_MATERIAL:
        *params = context->lightingInfo.isColorMaterial;
    break;

    case NMGL_LIGHT0:
	case NMGL_LIGHT1:
	case NMGL_LIGHT2:
	case NMGL_LIGHT3:
	case NMGL_LIGHT4:
	case NMGL_LIGHT5:
	case NMGL_LIGHT6:
	case NMGL_LIGHT7:
        *params = context->lightingInfo.isEnabledLight[pname - NMGL_LIGHT0];
    break;

    case NMGL_POINT_SMOOTH:
		*params = context->point.smooth_enabled;
	break;

    case NMGL_LINE_SMOOTH:
		*params = context->line.smooth_enabled;
	break;

    case NMGL_LINE_STIPPLE:
		*params = context->line.stipple.enabled;
	break;

    case NMGL_CULL_FACE:
		*params = context->isCullFace;
	break;

    case NMGL_POLYGON_OFFSET_FILL:
		*params = context->polygon.offset_fill_enabled;
	break;

    case NMGL_POLYGON_STIPPLE:
		*params = context->polygon.stipple.enabled;
	break;

    case NMGL_TEXTURE_2D:
		*params = cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled;
	break;

    case NMGL_SCISSOR_TEST:
		*params = context->scissorTest.isEnabled;
    break;
    case NMGL_ALPHA_TEST:
		*params =  context->alpha_test.enabled;
    break;
    case NMGL_STENCIL_TEST:
        *params =  context->stencil_test.enabled;
    break;
    case NMGL_DEPTH_TEST:
        *params =  context->depth_test.enabled;//TODO:add context field
    break;
    case NMGL_BLEND:
        *params =  context->blend.enabled ;
    break;
    default:
        cntxt->error = NMGL_INVALID_ENUM;
    break;
    }
}