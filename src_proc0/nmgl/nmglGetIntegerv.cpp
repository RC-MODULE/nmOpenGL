#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglGetIntegerv(NMGLenum pname, NMGLint * params) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();
	switch(pname){
		case NMGL_SHADE_MODEL:
			params[0] = (NMGLint)context->shadeModel;
			break;
		case NMGL_UNPACK_ALIGNMENT:
			params[0] = (NMGLint)cntxt->unpackAlignment;
			break;
		case NMGL_PACK_ALIGNMENT:
			params[0] = (NMGLint)cntxt->packAlignment;
			break;
		default:
			cntxt->error = NMGL_INVALID_ENUM;
	}
}
