#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglCullFace(NMGLenum mode){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if(mode != NMGL_BACK && mode != NMGL_FRONT){
		cntxt->error = NMGL_INVALID_ENUM ;
	} else{
		cntxt->cullFaceType = mode;
	}
	
}
