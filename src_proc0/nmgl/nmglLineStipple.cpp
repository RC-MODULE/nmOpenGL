#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")

void nmglLineStipple (NMGLint factor, NMGLushort pattern) {

    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    NMGL_Context *context = NMGL_GetCurrentContext();

    if( factor <= 0 )  {cntxt->error=NMGL_INVALID_VALUE;return;}
    
    if( factor > 256 ){
        context->line.stipple.factor = factor % 256;
        if( factor == 0 )  context->line.stipple.factor = 256;
    } 
    else {context->line.stipple.factor = factor;}

    if (context->line.stipple.enabled == NMGL_TRUE){

         context->line.stipple.pattern = pattern & 0xffff;
    }
    else
    {
        context->line.stipple.pattern = 0xffff;
    }

}
