#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

void nmglvsExit_mc12101() {
	LOCK_NMC1();
	ADD_INSTR_NMC1(NMC1_EXIT);
	UNLOCK_NMC1();
}