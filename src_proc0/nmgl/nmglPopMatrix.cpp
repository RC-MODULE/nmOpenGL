#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"
SECTION(".text_nmgl")
void nmglPopMatrix() {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if(context->currentMatrixStack->current == 0){
		cntxt->error = NMGL_STACK_UNDERFLOW;
		return;
	}
	context->currentMatrixStack->current--;

	if (context->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
		reverseMatrix3x3in4x4(context->modelviewMatrixStack.top(), &context->normalMatrix);
	}
	return;
}
