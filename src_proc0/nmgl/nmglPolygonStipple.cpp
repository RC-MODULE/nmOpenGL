#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")

void nmglPolygonStipple (const NMGLubyte *mask) {

    int i=0;

    NMGL_Context *cntxt = NMGL_GetCurrentContext();

    for(i=0;i<(NMGL_POLIGON_STIPPLE_SIDE_UBYTES*(NMGL_POLIGON_STIPPLE_SIDE_UBYTES>>3));i++){

        *(cntxt->polygon.stipple.pattern + i) = *(mask + i);
    }
}
