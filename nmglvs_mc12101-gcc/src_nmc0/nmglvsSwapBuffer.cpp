#include "demo3d_nm0.h"
#include "nmgl.h"
#include "hal.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

void nmglvsSwapBuffer(){
	static int time = 0;
	static clock_t t0, t1;
	t1 = clock();
	cntxt.synchro.counter++;
	cntxt.synchro.time = t1-t0;
	t0 = clock();
	cntxt.synchro.writeInstr(1, NMC1_SWAP_BUFFER, 0, 0, 0, 0, 0, 0);
	
}