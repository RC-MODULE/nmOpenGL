#include "demo3d_nm0.h"
#include "nmblas.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"
SECTION(".text_nmgl")
void nmglPushMatrix() {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (cntxt->currentMatrixStack->current < cntxt->currentMatrixStack->size - 1) {
		nmblas_scopy(16, (float*)&cntxt->currentMatrixStack->base[cntxt->currentMatrixStack->size], 1,
			(float*)&cntxt->currentMatrixStack->base[cntxt->currentMatrixStack->size + 1], 1);
		cntxt->currentMatrixStack->current++;
	}
	else{
		cntxt->error = NMGL_STACK_OVERFLOW;
	}
	return;
}
