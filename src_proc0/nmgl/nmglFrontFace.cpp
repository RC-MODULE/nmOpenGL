#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>


SECTION(".text_nmgl")
void nmglFrontFace (NMGLenum dir){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	if (dir != NMGL_CW && dir != NMGL_CCW)
	{
		cntxt->error = NMGL_INVALID_ENUM ;
	}
	else
	{
		context->frontFaceOrientation = dir;
	}
}
