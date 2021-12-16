#include "demo3d_nm0.h"
#include "nmgl.h"
#include "lighting.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglLightf(NMGLenum color, NMGLenum pname, NMGLfloat param) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	LightingInfo* lightingInfo = &NMGL_GetCurrentContext()->lightingInfo;
	
	color -= NMGL_LIGHT0;
	if (color >= MAX_LIGHTS) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	switch (pname)
	{
	case NMGL_SPOT_EXPONENT:
		if (param < 0 || param>128) {
			cntxt->error = NMGL_INVALID_VALUE;
			break;
		}
		else {
			lightingInfo->lightSpotExp[color] = param;
		}
		break;
	case NMGL_SPOT_CUTOFF:
		if ((param < 0 || param>90) && param != 180) {
			cntxt->error = NMGL_INVALID_VALUE;
			break;
		}
		else {
			lightingInfo->lightSpotCutoff [color] = param;
		}
		break;
	case NMGL_CONSTANT_ATTENUATION:
		lightingInfo->lightConstAtt[color] = param;
		break;
	case NMGL_LINEAR_ATTENUATION:
		lightingInfo->lightLinAtt[color] = param;
		break;
	case NMGL_QUADRATIC_ATTENUATION:
		lightingInfo->lightQuadAtt[color] = param;
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		break;
	}
	
}
