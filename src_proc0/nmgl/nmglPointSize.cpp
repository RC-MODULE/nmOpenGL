#include "demo3d_nm0.h"
#include "nmgl.h"
#include "math.h"
#include "nmglservice_nm0.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglPointSize(NMGLfloat size) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (size <= 0) {
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	context->pointSize;
}
