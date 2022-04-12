#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglLightModelfv(NMGLenum pname, const NMGLfloat *params) {
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if(pname != NMGL_LIGHT_MODEL_AMBIENT){
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	cntxt->lightingInfo.lightAmbient[MAX_LIGHTS].vec[0] = params[0];
	cntxt->lightingInfo.lightAmbient[MAX_LIGHTS].vec[1] = params[1];
	cntxt->lightingInfo.lightAmbient[MAX_LIGHTS].vec[2] = params[2];
	cntxt->lightingInfo.lightAmbient[MAX_LIGHTS].vec[3] = params[3];
}
