#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include <math.h>


SECTION(".text_nmgl")
void nmglDepthFunc (NMGLenum func){
	if (func >= NMGL_NEVER && func <= NMGL_ALWAYS) {
		addInstrNMC1(&cntxt.synchro->commandsRB, NMC1_DEPTH_FUNC, func);
	}
	else {
		cntxt.error = NMGL_INVALID_ENUM;
	}
}
