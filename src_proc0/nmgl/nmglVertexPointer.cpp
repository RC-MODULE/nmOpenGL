#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglVertexPointer(NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer) {
	if (size < 2 || size > 4) {
		cntxt.error = NMGL_INVALID_VALUE;
		return;
	}
	if (type != NMGL_FLOAT) {
		cntxt.error = NMGL_INVALID_ENUM;
		return;
	}
	if (stride < 0) {
		cntxt.error = NMGL_INVALID_VALUE;
		return;
	}
	cntxt.vertexArray.pointer = pointer;
	cntxt.vertexArray.size = size;
	cntxt.vertexArray.stride = stride;
	cntxt.vertexArray.type = type;
}