#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglLightModelfv(NMGLenum pname, const NMGLfloat *params) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	LightingInfo *lightingInfo = &NMGL_GetCurrentContext()->lightingInfo;

	if(pname != NMGL_LIGHT_MODEL_AMBIENT){
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	lightingInfo->lightAmbient[MAX_LIGHTS].vec[0] = params[0];
	lightingInfo->lightAmbient[MAX_LIGHTS].vec[1] = params[1];
	lightingInfo->lightAmbient[MAX_LIGHTS].vec[2] = params[2];
	lightingInfo->lightAmbient[MAX_LIGHTS].vec[3] = params[3];
}
