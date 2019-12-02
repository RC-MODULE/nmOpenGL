#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

void nmglvsExit_mc12101() {
	cntxt.synchro.writeInstr(1, NMC1_EXIT, 0, 0, 0, 0, 0, 0);
}