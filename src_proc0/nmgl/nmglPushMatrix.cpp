#include "demo3d_nm0.h"
#include "nmblas.h"
#include "nmgl.h"
#include "debugprint.h"


#pragma code_section ".text_nmgl"
SECTION(".text_nmgl")
void nmglPushMatrix() {
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (cntxt->currentMatrixStack->current < cntxt->currentMatrixStack->size - 1) {
		nmblas_scopy(16, (float*)&cntxt->currentMatrixStack->base[cntxt->currentMatrixStack->current], 1,
			(float*)&cntxt->currentMatrixStack->base[cntxt->currentMatrixStack->current + 1], 1);
		cntxt->currentMatrixStack->current++;
		if (cntxt->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
			reverseMatrix3x3in4x4(cntxt->modelviewMatrixStack.top(), &cntxt->normalMatrix);
		}
	}
	else{
		cntxt->error = NMGL_STACK_OVERFLOW;
	}
	return;
}
