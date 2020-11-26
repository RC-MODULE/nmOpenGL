#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglMultMatrixf(const NMGLfloat * m) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	mat4nm32f* current = cntxt->currentMatrixStack->top();
	mul_mat4nm32f_v4nm32f(current, (v4nm32f*)m, (v4nm32f*)current, 4);

	if (cntxt->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
		reverseMatrix3x3in4x4(cntxt->modelviewMatrixStack.top(), &cntxt->normalMatrix);
	}
}