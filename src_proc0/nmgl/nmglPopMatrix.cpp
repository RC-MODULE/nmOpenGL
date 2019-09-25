#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"
SECTION(".text_nmgl")
void nmglPopMatrix() {
	if(cntxt.currentMatrixStack->current == 0){
		cntxt.error = NMGL_STACK_UNDERFLOW;
		return;
	}
	cntxt.currentMatrixStack->current--;
	return;
}
