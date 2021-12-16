#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglShadeModel (NMGLenum mode){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();
	
	if ((mode == NMGL_FLAT) || (mode == NMGL_SMOOTH)) {

		context->shadeModel = mode;	

		NM_Command command;
	}
	else {
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
