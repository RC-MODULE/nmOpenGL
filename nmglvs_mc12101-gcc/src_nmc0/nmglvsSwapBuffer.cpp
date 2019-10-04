#include "demo3d_nm0.h"
#include "nmgl.h"
#include "hal.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

void nmglvsSwapBuffer(){
	static int time = 0;
	static clock_t t0, t1;
	t1 = clock();
	cntxt.synchro->counter_nmc0++;
	cntxt.synchro->time0 = t1-t0;
	t0 = clock();
	if (halRingBufferIsFull(&cntxt.synchro->commandsRB)) {
		while (!halRingBufferIsEmpty(&cntxt.synchro->commandsRB));
	}
	addInstrNMC1(&cntxt.synchro->commandsRB, NMC1_SWAP_BUFFER);
	
}