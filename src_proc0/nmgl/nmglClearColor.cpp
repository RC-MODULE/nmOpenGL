#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "imagebuffer.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClearColor(NMGLclampf red, NMGLclampf green, NMGLclampf blue, NMGLclampf alpha) {
	cntxt.synchro.writeInstr(1, NMC1_SET_COLOR, 
		(int)(red * RED_COEFF), 
		(int)(green * GREEN_COEFF),
		(int)(blue * BLUE_COEFF),
		(int)(alpha * ALPHA_COEFF));
}

