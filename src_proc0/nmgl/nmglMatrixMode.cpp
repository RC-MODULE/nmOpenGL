#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglMatrixMode(NMGLenum mode) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	switch (mode) {
	case NMGL_MODELVIEW:
		context->currentMatrixStack = &context->modelviewMatrixStack;
		break;
	case NMGL_PROJECTION:
		context->currentMatrixStack = &context->projectionMatrixStack;
		break;
	case NMGL_TEXTURE:
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
