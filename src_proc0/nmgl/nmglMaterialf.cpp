#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglMaterialf(NMGLenum face, NMGLenum pname, NMGLfloat param) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if(face != NMGL_FRONT_AND_BACK || pname != NMGL_SHININESS){
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	if (param < 0 || param>128) {
		cntxt->error = NMGL_INVALID_VALUE;
	}
	else {
		cntxt->specularExp = param;
	}
}