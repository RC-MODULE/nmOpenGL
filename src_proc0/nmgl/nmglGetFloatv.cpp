#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglGetFloatv(NMGLenum pname, NMGLfloat * params) {
	switch(pname){
	case NMGL_MATRIX_MODE:
		params[0] = cntxt.currentMatrixStack->type;
		break;
	case NMGL_MODELVIEW_MATRIX:
		nmblas_scopy(16, (float*)cntxt.modelviewMatrixStack.top(),1,params,1);
	case NMGL_PROJECTION_MATRIX:
		nmblas_scopy(16, (float*)cntxt.projectionMatrixStack.top(), 1, params, 1);
	default:
		cntxt.error = NMGL_INVALID_ENUM;
	}
}
