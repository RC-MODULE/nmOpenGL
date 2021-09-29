#include "demo3d_nm0.h"
#include "nmgl.h"
#include "hal.h"


#pragma code_section ".text_nmgl"

void nmglvsSwapBuffer(){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	nmglFlush();
	//nmglFinish();

	NM_Command command;
	command.instr = NMC1_SWAP_BUFFER;
	cntxt->synchro.pushInstr(&command);
	
}