#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglVertexPointer(NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (size < 2 || size > 4) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	if (type != NMGL_FLOAT) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if (stride < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	context->vertexArray.pointer = pointer;
	context->vertexArray.size = size;
	context->vertexArray.stride = stride;
	context->vertexArray.type = type;
}