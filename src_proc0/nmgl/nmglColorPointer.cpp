#include "demo3d_nm0.h"
#include "nmgl.h"


SECTION(".text_nmgl")
void nmglColorPointer(NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if(size!=4){
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	if (type != NMGL_FLOAT && type != NMGL_UNSIGNED_BYTE) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if (stride < 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	context->colorArray.pointer = (float *)pointer;
	context->colorArray.size = 4;
	context->colorArray.stride = stride;
	context->colorArray.type = type;
}