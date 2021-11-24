#include "demo3d_nm0.h"
#include "nmgl.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")

void nmglGetLightfv (NMGLenum light , NMGLenum pname, NMGLfloat *params)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	LightingInfo* lightingInfo = &cntxt->lightingInfo;
	
	light -= NMGL_LIGHT0;
	
	if (light >= MAX_LIGHTS) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	switch (pname)
	{
	case NMGL_AMBIENT:
		params[0] = lightingInfo->lightAmbient[light].vec[0];
		params[1] = lightingInfo->lightAmbient[light].vec[1];
		params[2] = lightingInfo->lightAmbient[light].vec[2];
		params[3] = lightingInfo->lightAmbient[light].vec[3];
		break;
	case NMGL_DIFFUSE:
		params[0] = lightingInfo->lightDiffuse[light].vec[0];
		params[1] = lightingInfo->lightDiffuse[light].vec[1];
		params[2] = lightingInfo->lightDiffuse[light].vec[2];
		params[3] = lightingInfo->lightDiffuse[light].vec[3];
		break;
	case NMGL_SPECULAR:
		params[0] = lightingInfo->lightSpecular[light].vec[0];
		params[1] = lightingInfo->lightSpecular[light].vec[1];
		params[2] = lightingInfo->lightSpecular[light].vec[2];
		params[3] = lightingInfo->lightSpecular[light].vec[3];
		break;
	case NMGL_POSITION:
		params[0] = lightingInfo->lightPosition[light].vec[0];
		params[1] = lightingInfo->lightPosition[light].vec[1];
		params[2] = lightingInfo->lightPosition[light].vec[2];
		params[3] = lightingInfo->lightPosition[light].vec[3];
		break;
	case NMGL_SPOT_DIRECTION:
		params[0] = lightingInfo->lightSpotDirection[light].vec[0];
		params[1] = lightingInfo->lightSpotDirection[light].vec[1];
		params[2] = lightingInfo->lightSpotDirection[light].vec[2];
		params[3] = 0;
		break;
	case NMGL_SPOT_EXPONENT:
		params[0] = lightingInfo->lightSpotExp[light];
		break;
	case NMGL_SPOT_CUTOFF:
		params[0]= lightingInfo->lightSpotCutoff[light];
		break;
	case NMGL_CONSTANT_ATTENUATION:
		params[0] = lightingInfo->lightConstAtt[light];
		break;
	case NMGL_LINEAR_ATTENUATION:
		params[0] = lightingInfo->lightLinAtt[light];
		break;
	case NMGL_QUADRATIC_ATTENUATION:
		params[0] = lightingInfo->lightQuadAtt[light];
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		break;
	}
}
