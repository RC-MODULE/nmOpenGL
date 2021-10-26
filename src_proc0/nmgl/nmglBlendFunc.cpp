#include "demo3d_nm0.h"
#include "nmgl.h"


void nmglBlendFunc (NMGLenum sfactor, NMGLenum dfactor){
    
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    switch(sfactor)
    {
        case NMGL_ONE:
            if (dfactor != NMGL_ZERO) {
              cntxt->error = NMGL_INVALID_ENUM;
            return;                  
            }
            break;
        case NMGL_SRC_ALPHA:
            if (dfactor != NMGL_ONE_MINUS_SRC_ALPHA) {
                cntxt->error = NMGL_INVALID_ENUM;
                return;                
            }
            break;
        case NMGL_SRC_ALPHA_SATURATE:
            if (dfactor != NMGL_ONE) {
                cntxt->error = NMGL_INVALID_ENUM;
                return;                
            }
            break;
        
        default:
            cntxt->error = NMGL_INVALID_ENUM;
            return;
    }    
   
    cntxt->blend.sfactor = sfactor;
    cntxt->blend.dfactor = dfactor;
   




}