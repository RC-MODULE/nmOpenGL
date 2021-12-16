#include "demo3d_nm0.h"
#include "nmgl.h"
#include "math.h"
void nmglAlphaFunc (NMGLenum func, NMGLclampf ref){
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    NMGL_Context *context = NMGL_GetCurrentContext();

    if ((func != NMGL_LEQUAL)&&(func != NMGL_ALWAYS)){
        cntxt->error = NMGL_INVALID_ENUM;
        return;
    }

    context->alpha_test.func = func;
    if(ref <= 0.0) context->alpha_test.ref = 0.0;
    else{
        if(ref > 1.0) context->alpha_test.ref = 1.0;
        else context->alpha_test.ref = ref;
    }
}


