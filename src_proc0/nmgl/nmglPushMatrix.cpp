#include "demo3d_nm0.h"
#include "nmblas.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"
SECTION(".text_nmgl")
void nmglPushMatrix() {
	if (cntxt.currentMatrixStack->current < cntxt.currentMatrixStack->size - 1) {
		nmblas_scopy(16, (float*)&cntxt.currentMatrixStack->base[cntxt.currentMatrixStack->size], 1,
			(float*)&cntxt.currentMatrixStack->base[cntxt.currentMatrixStack->size + 1], 1);
		cntxt.currentMatrixStack->current++;
	}
	else{
		cntxt.error = NMGL_STACK_OVERFLOW;
	}
	return;
}
