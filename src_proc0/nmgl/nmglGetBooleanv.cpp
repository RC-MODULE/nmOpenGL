#include "demo3d_nm0.h"
#include "nmgl.h"


void nmglGetBooleanv (NMGLenum pname, NMGLboolean *params){

    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	switch(pname){
	case NMGL_SHARED_TEXTURE_PALETTE_EXT:
    break;

    case NMGL_COLOR_WRITEMASK:
    break;

    case NMGL_DEPTH_WRITEMASK:
    break;

    default:
    break;
    }
}