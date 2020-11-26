#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"
SECTION(".text_nmgl")
void nmglPopMatrix() {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if(cntxt->currentMatrixStack->current == 0){
		cntxt->error = NMGL_STACK_UNDERFLOW;
		return;
	}
	cntxt->currentMatrixStack->current--;

	if (cntxt->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
		reverseMatrix3x3in4x4(cntxt->modelviewMatrixStack.top(), &cntxt->normalMatrix);
	}
	return;
}
