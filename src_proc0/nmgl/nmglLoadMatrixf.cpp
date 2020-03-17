#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglLoadMatrixf(const NMGLfloat *m){
	for(int i=0;i< 16; i++){
		cntxt.currentMatrixStack->top()->matr[i] = m[i];
	}
}
