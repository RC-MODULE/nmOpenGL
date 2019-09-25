#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmblas.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglGetIntegerv(NMGLenum pname, NMGLint * params) {
	switch(pname){
		
	default:
		cntxt.error = NMGL_INVALID_ENUM;
	}
}
