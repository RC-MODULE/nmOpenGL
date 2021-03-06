#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglDepthFunc (NMGLenum func){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if (func >= NMGL_NEVER && func <= NMGL_ALWAYS) {
		cntxt->synchro.writeInstr(1, NMC1_DEPTH_FUNC, func);
	}
	else {
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
