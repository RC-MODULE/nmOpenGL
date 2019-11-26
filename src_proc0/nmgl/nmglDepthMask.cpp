#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include <math.h>


SECTION(".text_nmgl")
void nmglDepthMask (NMGLboolean flag){
	cntxt.synchro->writeInstr(1, NMC1_DEPTH_MASK, flag);
}
