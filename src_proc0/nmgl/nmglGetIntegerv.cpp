#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglGetIntegerv(NMGLenum pname, NMGLint * params) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	switch(pname){
		case NMGL_SHADE_MODEL:
			params[0] = (NMGLint)cntxt->shadeModel;
			break;
		case NMGL_UNPACK_ALIGNMENT:
			params[0] = (NMGLint)cntxt->unpackAlignment;
			break;
		case NMGL_PACK_ALIGNMENT:
			params[0] = (NMGLint)cntxt->packAlignment;
			break;
		case NMGL_PERSPECTIVE_CORRECTION_HINT: 
			params[0] = (NMGLint)cntxt->perspectiveCorrectionHint;
			break;
		case NMGL_LINE_SMOOTH_HINT:
			params[0] = (NMGLint)cntxt->lineSmoothHint;
			break;
		case NMGL_POINT_SMOOTH_HINT:
			params[0] = (NMGLint)cntxt->pointSmoothHint;
			break;
		default:
			cntxt->error = NMGL_INVALID_ENUM;
	}
}

