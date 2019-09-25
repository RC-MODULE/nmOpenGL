#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglMatrixMode(NMGLenum mode) {
	switch (mode) {
	case NMGL_MODELVIEW:
		cntxt.currentMatrixStack = &cntxt.modelviewMatrixStack;
		break;
	case NMGL_PROJECTION:
		cntxt.currentMatrixStack = &cntxt.projectionMatrixStack;
		break;
	case NMGL_TEXTURE:
		break;
	default:
		cntxt.error = NMGL_INVALID_ENUM;
	}
}
