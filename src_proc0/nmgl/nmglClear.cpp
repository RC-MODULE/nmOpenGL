#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClear(NMGLbitfield mask) {
	addInstrNMC1(&cntxt.synchro->commandsRB, NMC1_CLEAR, mask);
}
