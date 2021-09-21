#include "demo3d_nm0.h"
#include "nmgl.h"
#include "lighting.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglLightfv(NMGLenum color, NMGLenum pname, const NMGLfloat *params) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	LightingInfo* lightingInfo = &cntxt->lightingInfo;
	color -= NMGL_LIGHT0;
	if (color >= MAX_LIGHTS) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	switch (pname)
	{
	case NMGL_AMBIENT:
		lightingInfo->lightAmbient[color].vec[0] = params[0];
		lightingInfo->lightAmbient[color].vec[1] = params[1];
		lightingInfo->lightAmbient[color].vec[2] = params[2];
		lightingInfo->lightAmbient[color].vec[3] = params[3];
		break;
	case NMGL_DIFFUSE:
		lightingInfo->lightDiffuse[color].vec[0] = params[0];
		lightingInfo->lightDiffuse[color].vec[1] = params[1];
		lightingInfo->lightDiffuse[color].vec[2] = params[2];
		lightingInfo->lightDiffuse[color].vec[3] = params[3];
		break;
	case NMGL_SPECULAR:
		lightingInfo->lightSpecular[color].vec[0] = params[0];
		lightingInfo->lightSpecular[color].vec[1] = params[1];
		lightingInfo->lightSpecular[color].vec[2] = params[2];
		lightingInfo->lightSpecular[color].vec[3] = params[3];
		break;
	case NMGL_POSITION:
		cntxt->tmp.vec[0] = params[0];
		cntxt->tmp.vec[1] = params[1];
		cntxt->tmp.vec[2] = params[2];
		cntxt->tmp.vec[3] = params[3];
		mul_mat4nm32f_v4nm32f(cntxt->modelviewMatrixStack.top(), 
			&cntxt->tmp, 
			&lightingInfo->lightPosition[color], 1);
		break;
	case NMGL_SPOT_DIRECTION:
		lightingInfo->lightSpotDirection[color].vec[0] = params[0];
		lightingInfo->lightSpotDirection[color].vec[1] = params[1];
		lightingInfo->lightSpotDirection[color].vec[2] = params[2];
		lightingInfo->lightSpotDirection[color].vec[3] = 0;
		break;
	case NMGL_SPOT_EXPONENT:
		if (params[0] < 0 || params[0]>128) {
			cntxt->error = NMGL_INVALID_VALUE;
			break;
		}
		else {
			lightingInfo->lightSpotExp[color] = params[0];
		}
		break;
	case NMGL_SPOT_CUTOFF:
		if ((params[0] < 0 || params[0]>90) && params[0] != 180) {
			cntxt->error = NMGL_INVALID_VALUE;
			break;
		}
		else {
			lightingInfo->lightSpotCutoff[color] = params[0];
		}
		break;
	case NMGL_CONSTANT_ATTENUATION:
		lightingInfo->lightConstAtt[color] = params[0];
		break;
	case NMGL_LINEAR_ATTENUATION:
		lightingInfo->lightLinAtt[color] = params[0];
		break;
	case NMGL_QUADRATIC_ATTENUATION:
		lightingInfo->lightQuadAtt[color] = params[0];
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		break;
	}

}
