#include "demo3d_nm0.h"
#include "nmgl.h"
#include "math.h"
#include "debugprint.h"

void nmglAlphaFunc (NMGLenum func, NMGLclampf ref){
    DEBUG_PLOG_FUNCTION();
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    if ((func != NMGL_LEQUAL)&&(func != NMGL_ALWAYS)){
        cntxt->error = NMGL_INVALID_ENUM;
        return;
    }

    cntxt->alpha_test.func = func;
    if(ref <= 0.0) cntxt->alpha_test.ref = 0.0;
    else{
        if(ref > 1.0) cntxt->alpha_test.ref = 1.0;
        else cntxt->alpha_test.ref = ref;
    }
}


