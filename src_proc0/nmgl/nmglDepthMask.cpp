#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglDepthMask (NMGLboolean flag){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	cntxt->synchro.writeInstr(1, NMC1_DEPTH_MASK, flag);
}
