#include "demo3d_nm0.h"
#include "nmgl.h"
#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglGetPolygonStipple (NMGLubyte *mask){
	
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
    NMGL_Context *context = NMGL_GetCurrentContext();
	
    int i;

    for(i=0;i<(NMGL_POLIGON_STIPPLE_SIDE_UBYTES)*(NMGL_POLIGON_STIPPLE_SIDE_UBYTES>>3);i++)
    {
        *(context->polygon.stipple.pattern + i) = *(mask + i);
    }

}
