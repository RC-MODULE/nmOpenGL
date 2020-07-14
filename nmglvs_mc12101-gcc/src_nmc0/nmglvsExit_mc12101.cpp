#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

void nmglvsExit_mc12101() {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->synchro.writeInstr(1, NMC1_EXIT, 0, 0, 0, 0, 0, 0);
}