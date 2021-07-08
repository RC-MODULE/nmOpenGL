#include "demo3d_nm0.h"
#include "nmgl.h"
#include "hal.h"


#pragma code_section ".text_nmgl"

void nmglvsSwapBuffer(){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	nmglFlush();

	static int time = 0;
	static clock_t t0, t1;
	t1 = clock();
	cntxt->synchro.counter++;
	cntxt->synchro.time = t1-t0;
	t0 = clock();
	CommandNm1 command;
	command.instr = NMC1_SWAP_BUFFER;
	cntxt->synchro.pushInstr(&command);
	
}