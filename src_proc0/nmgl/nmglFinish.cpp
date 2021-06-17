#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglFinish (){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	CommandNm1 command;
	command.instr = NMC1_FINISH;
	cntxt->synchro.pushInstr(&command);
	while (!cntxt->synchro.isEmpty());
}
