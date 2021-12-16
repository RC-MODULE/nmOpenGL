#include "demo3d_nm0.h"
#include "nmblas.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"
SECTION(".text_nmgl")
void nmglPushMatrix() {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (context->currentMatrixStack->current < context->currentMatrixStack->size - 1) {
		nmblas_scopy(16, (float *)&context->currentMatrixStack->base[context->currentMatrixStack->size], 1,
					 (float *)&context->currentMatrixStack->base[context->currentMatrixStack->size + 1], 1);
		context->currentMatrixStack->current++;
		if (context->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
			reverseMatrix3x3in4x4(context->modelviewMatrixStack.top(), &context->normalMatrix);
		}
	}
	else{
		cntxt->error = NMGL_STACK_OVERFLOW;
	}
	return;
}
