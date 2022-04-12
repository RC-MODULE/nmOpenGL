#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"

void nmglPixelStorei (NMGLenum pname, NMGLint param)
{
    DEBUG_PLOG_FUNCTION();
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
    NMGLint *alignment;
    
    switch(pname){
		case NMGL_UNPACK_ALIGNMENT://1,2,4,8            
			alignment = &cntxt->unpackAlignment;
            break;
		case NMGL_PACK_ALIGNMENT://1,2,4,8            
			alignment = &cntxt->packAlignment;
            break;			
        default:
            cntxt->error=NMGL_INVALID_ENUM;return;
    }
    if (!((param == 1)||(param == 2)||(param == 4)||(param == 8)))
    {
        cntxt->error=NMGL_INVALID_VALUE;return;
    }
    else
    {
        *alignment = param;
    }
}