#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include <math.h>


SECTION(".text_nmgl")
void nmglCullFace(NMGLenum mode){
	if(mode != NMGL_BACK && mode != NMGL_FRONT){
		cntxt.error = NMGL_INVALID_ENUM ;
	} else{
		cntxt.cullFaceType = mode;
	}
	
}
