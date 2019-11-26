#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClearColor(NMGLclampf red, NMGLclampf green, NMGLclampf blue, NMGLclampf alpha) {
	cntxt.synchro->writeInstr(1, NMC1_SET_COLOR, (int)(red*255.0f), (int)(green*255.0f), (int)(blue*255.0f), (int)(alpha*255.0f));
}

