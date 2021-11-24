#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

NMGLboolean nmglIsEnabled(NMGLenum cap)
{
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    switch (cap) {

	case NMGL_SHARED_TEXTURE_PALETTE_EXT:
		return cntxt->texState.shared_palette_enabled;		

    case NMGL_VERTEX_ARRAY:
        return cntxt->vertexArray.enabled;

    case NMGL_COLOR_ARRAY:
        return cntxt->colorArray.enabled;

    case NMGL_NORMAL_ARRAY:
        return cntxt->normalArray.enabled;
        
    case NMGL_TEXTURE_COORD_ARRAY:
        return cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnitIndex].enabled;
        
    case NMGL_NORMALIZE:
        return cntxt->normalizeEnabled;
    
    case NMGL_RESCALE_NORMAL:
        return NMGL_FALSE;
    
    case NMGL_LIGHTING:
        return cntxt->lightingInfo.isLighting;

    case NMGL_COLOR_MATERIAL:
        return cntxt->lightingInfo.isColorMaterial;

    case NMGL_LIGHT0:
	case NMGL_LIGHT1:
	case NMGL_LIGHT2:
	case NMGL_LIGHT3:
	case NMGL_LIGHT4:
	case NMGL_LIGHT5:
	case NMGL_LIGHT6:
	case NMGL_LIGHT7:
	    return cntxt->lightingInfo.isEnabledLight[cap - NMGL_LIGHT0];

    case NMGL_POINT_SMOOTH:    
        return cntxt->point.smooth_enabled;

    case NMGL_LINE_SMOOTH:    
        return cntxt->line.smooth_enabled;

    case NMGL_LINE_STIPPLE:    
        return cntxt->line.stipple.enabled;

    case NMGL_CULL_FACE:
       return cntxt->isCullFace;
    
    case NMGL_POLYGON_OFFSET_FILL:
       return cntxt->polygon.offset_fill_enabled;

    case NMGL_POLYGON_STIPPLE:
       return cntxt->polygon.stipple.enabled;

    case NMGL_TEXTURE_2D:
        return cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled;

    case NMGL_SCISSOR_TEST:
		return cntxt->scissorTest.isEnabled;

    case NMGL_ALPHA_TEST:
		return cntxt->alpha_test.enabled;

    case NMGL_STENCIL_TEST:
        return cntxt->stencil_test.enabled;

    case NMGL_DEPTH_TEST:
        return cntxt->depth_test.enabled;//TODO:add context field

    case NMGL_BLEND:
        return cntxt->blend.enabled;


    default:
        return NMGL_FALSE;
    }
}
