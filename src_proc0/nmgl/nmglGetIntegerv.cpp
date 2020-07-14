#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglGetIntegerv(NMGLenum pname, NMGLint * params) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	switch(pname){
		
	default:
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
