#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"


void nmglGetBooleanv (NMGLenum pname, NMGLboolean *params){
    DEBUG_PLOG_FUNCTION();
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	switch(pname){
	case NMGL_SHARED_TEXTURE_PALETTE_EXT:
        *params = cntxt->texState.shared_palette_enabled;
    break;

    case NMGL_COLOR_WRITEMASK:
        int i;
        for(i=0;i<4;i++){
            params[i] = cntxt->color_write_mask_flags[i];
        }
    break;

    case NMGL_DEPTH_WRITEMASK:
        *params = cntxt->depth_write_mask_enabled;
    break;

    case NMGL_VERTEX_ARRAY:
        *params = cntxt->vertexArray.enabled;
    break;

    case NMGL_NORMAL_ARRAY:
        *params = cntxt->normalArray.enabled;
    break;

    case NMGL_COLOR_ARRAY:
        *params = cntxt->colorArray.enabled;
    break;

    case NMGL_TEXTURE_COORD_ARRAY:
        *params = cntxt->texState.texcoordArray[cntxt->texState.activeTexUnit].enabled;
    break;

    case NMGL_NORMALIZE:
        *params = cntxt->normalizeEnabled;
    break;

    case NMGL_RESCALE_NORMAL:
        *params = NMGL_FALSE;
    break;
    
    case NMGL_LIGHTING:
        *params = cntxt->lightingInfo.isLighting;
    break;

    case NMGL_COLOR_MATERIAL:
        *params = cntxt->lightingInfo.isColorMaterial;
    break;

    case NMGL_LIGHT0:
	case NMGL_LIGHT1:
	case NMGL_LIGHT2:
	case NMGL_LIGHT3:
	case NMGL_LIGHT4:
	case NMGL_LIGHT5:
	case NMGL_LIGHT6:
	case NMGL_LIGHT7:
        *params = cntxt->lightingInfo.isEnabledLight[pname - NMGL_LIGHT0];
    break;

    case NMGL_POINT_SMOOTH:
		*params = cntxt->point.smooth_enabled;
	break;

    case NMGL_LINE_SMOOTH:
		*params = cntxt->line.smooth_enabled;
	break;

    case NMGL_LINE_STIPPLE:
		*params = cntxt->line.stipple.enabled;
	break;

    case NMGL_CULL_FACE:
		*params = cntxt->isCullFace;
	break;

    case NMGL_POLYGON_OFFSET_FILL:
		*params = cntxt->polygon.offset_fill_enabled;
	break;

    case NMGL_POLYGON_STIPPLE:
		*params = cntxt->polygon.stipple.enabled;
	break;

    case NMGL_TEXTURE_2D:
		*params = cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled;
	break;

    case NMGL_SCISSOR_TEST:
		*params = cntxt->scissorTest.isEnabled;
    break;
    case NMGL_ALPHA_TEST:
		*params =  cntxt->alpha_test.enabled;
    break;
    case NMGL_STENCIL_TEST:
        *params =  cntxt->stencil_test.enabled;
    break;
    case NMGL_DEPTH_TEST:
        *params =  cntxt->depth_test.enabled;//TODO:add context field
    break;
    case NMGL_BLEND:
        *params =  cntxt->blend.enabled ;
    break;
    default:
        cntxt->error = NMGL_INVALID_ENUM;
    break;
    }
}