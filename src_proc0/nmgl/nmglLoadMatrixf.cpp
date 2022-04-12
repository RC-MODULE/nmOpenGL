#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglLoadMatrixf(const NMGLfloat *m){
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	for(int i=0;i< 16; i++){
		cntxt->currentMatrixStack->top()->matr[i] = m[i];
	}

	if (cntxt->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
		reverseMatrix3x3in4x4(cntxt->modelviewMatrixStack.top(), &cntxt->normalMatrix);
	}
}
