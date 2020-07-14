#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglMatrixMode(NMGLenum mode) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	switch (mode) {
	case NMGL_MODELVIEW:
		cntxt->currentMatrixStack = &cntxt->modelviewMatrixStack;
		break;
	case NMGL_PROJECTION:
		cntxt->currentMatrixStack = &cntxt->projectionMatrixStack;
		break;
	case NMGL_TEXTURE:
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
