#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")

void nmglPolygonOffset (NMGLfloat factor, NMGLfloat units) {

    NMGL_Context *cntxt = NMGL_GetCurrentContext();

    cntxt->polygon.offset.factor = factor;
    cntxt->polygon.offset.units = units;
    
}
