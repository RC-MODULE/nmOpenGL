#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClear(NMGLbitfield mask) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->synchro.writeInstr(1, NMC1_CLEAR, mask);
}
