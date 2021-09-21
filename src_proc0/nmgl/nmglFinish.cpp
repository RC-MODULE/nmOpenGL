#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglFinish (){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NM_Command command;
	command.instr = NMC1_FINISH;
	NMGL_SetValue(command);
	while (!cntxt->synchro.isEmpty());
}
