#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

NMGLboolean nmglIsEnabled(NMGLenum cap)
{
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    switch (cap) {

	case NMGL_SHARED_TEXTURE_PALETTE_EXT:
		return cntxt->texState.using_shared_palette;		

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
        return NMGL_FALSE;

    case NMGL_LINE_SMOOTH:    
        return NMGL_FALSE;

    case NMGL_LINE_STIPPLE:    
        return NMGL_FALSE;

    case NMGL_CULL_FACE:
       return cntxt->isCullFace;
    
    case NMGL_POLYGON_OFFSET_FILL:
       return NMGL_FALSE;

    case NMGL_POLYGON_STIPPLE:
       return NMGL_FALSE;

    case NMGL_TEXTURE_2D:
        return cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].enabled;

    case NMGL_SCISSOR_TEST:
		return cntxt->scissorTest.isEnabled;

    case NMGL_ALPHA_TEST:
		return NMGL_FALSE;

    case NMGL_STENCIL_TEST:
        return NMGL_FALSE;

    case NMGL_DEPTH_TEST:
        return NMGL_FALSE;//TODO:add context field

    case NMGL_BLEND:
        return NMGL_FALSE;


    default:
        return NMGL_FALSE;
    }
}
