#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglFrontFace (NMGLenum dir){
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if(dir != NMGL_CW && dir != NMGL_CCW){
		cntxt->error = NMGL_INVALID_ENUM ;
	} else{
		cntxt->frontFaceOrientation = dir;
	}
	
}
