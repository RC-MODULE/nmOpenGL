#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")

void nmglLineStipple (NMGLint factor, NMGLushort pattern) {
    DEBUG_PLOG_FUNCTION();
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

    if( factor <= 0 )  {cntxt->error=NMGL_INVALID_VALUE;return;}
    
    if( factor > 256 ){
        cntxt->line.stipple.factor = factor % 256;
        if( factor == 0 )  cntxt->line.stipple.factor = 256;
    } 
    else {cntxt->line.stipple.factor = factor;}

    if (cntxt->line.stipple.enabled == NMGL_TRUE){

         cntxt->line.stipple.pattern = pattern & 0xffff;
    }
    else
    {
        cntxt->line.stipple.pattern = 0xffff;
    }

}
