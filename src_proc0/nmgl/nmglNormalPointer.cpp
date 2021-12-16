#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglNormalPointer(NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (type != NMGL_FLOAT) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if (stride < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	context->normalArray.pointer = pointer;
	context->normalArray.size = 3;
	context->normalArray.stride = stride;
	context->normalArray.type = type;
}

SECTION(".text_nmgl")
void nmglNormalPointerNM(NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (type != NMGL_FLOAT) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if (stride < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	context->normalArray.pointer = pointer;
	context->normalArray.size = 4;
	context->normalArray.stride = stride;
	context->normalArray.type = type;
}