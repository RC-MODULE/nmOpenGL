#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

NMGLboolean nmglIsEnabled(NMGLenum cap)
{
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    NMGL_Context *context = NMGL_GetCurrentContext();

    switch (cap) {

	case NMGL_SHARED_TEXTURE_PALETTE_EXT:
		return cntxt->texState.shared_palette_enabled;		

    case NMGL_VERTEX_ARRAY:
        return context->vertexArray.enabled;

    case NMGL_COLOR_ARRAY:
        return context->colorArray.enabled;

    case NMGL_NORMAL_ARRAY:
        return context->normalArray.enabled;

    case NMGL_TEXTURE_COORD_ARRAY:
        return cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnitIndex].enabled;
        
    case NMGL_NORMALIZE:
        return context->normalizeEnabled;

    case NMGL_RESCALE_NORMAL:
        return NMGL_FALSE;
    
    case NMGL_LIGHTING:
        return context->lightingInfo.isLighting;

    case NMGL_COLOR_MATERIAL:
        return context->lightingInfo.isColorMaterial;

    case NMGL_LIGHT0:
	case NMGL_LIGHT1:
	case NMGL_LIGHT2:
	case NMGL_LIGHT3:
	case NMGL_LIGHT4:
	case NMGL_LIGHT5:
	case NMGL_LIGHT6:
	case NMGL_LIGHT7:
	    return context->lightingInfo.isEnabledLight[cap - NMGL_LIGHT0];

    case NMGL_POINT_SMOOTH:
        return context->point.smooth_enabled;

    case NMGL_LINE_SMOOTH:
        return context->line.smooth_enabled;

    case NMGL_LINE_STIPPLE:
        return context->line.stipple.enabled;

    case NMGL_CULL_FACE:
        return context->isCullFace;

    case NMGL_POLYGON_OFFSET_FILL:
        return context->polygon.offset_fill_enabled;

    case NMGL_POLYGON_STIPPLE:
        return context->polygon.stipple.enabled;

    case NMGL_TEXTURE_2D:
        return cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled;

    case NMGL_SCISSOR_TEST:
        return context->scissorTest.isEnabled;

    case NMGL_ALPHA_TEST:
        return context->alpha_test.enabled;

    case NMGL_STENCIL_TEST:
        return context->stencil_test.enabled;

    case NMGL_DEPTH_TEST:
        return context->depth_test.enabled; //TODO:add context field

    case NMGL_BLEND:
        return context->blend.enabled;

    default:
        return NMGL_FALSE;
    }
}
