#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")

void nmglPolygonOffset (NMGLfloat factor, NMGLfloat units) {
    DEBUG_PLOG_FUNCTION();
    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

    cntxt->polygon.offset.factor = factor;
    cntxt->polygon.offset.units = units;
    
}
