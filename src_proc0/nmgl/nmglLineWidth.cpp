#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

void nmglLineWidth (NMGLfloat width)
{
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    NMGL_Context *context = NMGL_GetCurrentContext();

    if ( width > 0.0) context->line.width = width;
    else cntxt->error = NMGL_INVALID_VALUE;
}
