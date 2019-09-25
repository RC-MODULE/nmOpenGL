#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include <math.h>


SECTION(".text_nmgl")
void nmglFrontFace (NMGLenum dir){
	if(dir != NMGL_CW && dir != NMGL_CCW){
		cntxt.error = NMGL_INVALID_ENUM ;
	} else{
		cntxt.frontFaceOrientation = dir;
	}
	
}
