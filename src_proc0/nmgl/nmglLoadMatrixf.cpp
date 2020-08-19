#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglLoadMatrixf(const NMGLfloat *m){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	for(int i=0;i< 16; i++){
		cntxt->currentMatrixStack->top()->matr[i] = m[i];
	}
}
