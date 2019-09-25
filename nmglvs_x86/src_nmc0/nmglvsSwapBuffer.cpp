#include "demo3d_nm0.h"
#include "nmgl.h"
#include "hal.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

void nmglvsSwapBuffer(){
	LOCK_NMC1();
	nmglSynchro->counter_nmc0++;
	nmglSynchro->time0 = time_nmc0;
	time_nmc0 = 0;
	counter_measure_time0 = 0;
	ADD_INSTR_NMC1(NMC1_SWAP_BUFFER);
	UNLOCK_NMC1();
}