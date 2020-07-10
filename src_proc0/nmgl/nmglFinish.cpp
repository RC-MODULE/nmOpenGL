#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include <math.h>


SECTION(".text_nmgl")
void nmglFinish (){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	while (!cntxt->synchro.isEmpty());
}
