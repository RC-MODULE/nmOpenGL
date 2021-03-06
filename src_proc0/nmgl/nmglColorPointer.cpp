#include "demo3d_nm0.h"
#include "nmgl.h"


SECTION(".text_nmgl")
void nmglColorPointer(NMGLint size, NMGLenum type, NMGLsizei stride, const NMGLvoid *pointer) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
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
	cntxt->colorArray.pointer = (float*)pointer;
	cntxt->colorArray.size = 4;
	cntxt->colorArray.stride = stride;
	cntxt->colorArray.type = type;
}