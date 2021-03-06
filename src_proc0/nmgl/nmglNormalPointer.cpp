#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglNormalPointer(NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (type != NMGL_FLOAT) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if (stride < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	cntxt->normalArray.pointer = pointer;
	cntxt->normalArray.size = 3;
	cntxt->normalArray.stride = stride;
	cntxt->normalArray.type = type;
}

SECTION(".text_nmgl")
void nmglNormalPointerNM(NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (type != NMGL_FLOAT) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if (stride < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	cntxt->normalArray.pointer = pointer;
	cntxt->normalArray.size = 4;
	cntxt->normalArray.stride = stride;
	cntxt->normalArray.type = type;
}