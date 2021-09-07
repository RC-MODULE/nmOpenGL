#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglLightfv(NMGLenum color, NMGLenum pname, const NMGLfloat *params) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	color -= NMGL_LIGHT0;
	if (color >= MAX_LIGHTS) {
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	switch (pname)
	{
	case NMGL_AMBIENT:
		cntxt->lightAmbient[color].vec[0] = params[0];
		cntxt->lightAmbient[color].vec[1] = params[1];
		cntxt->lightAmbient[color].vec[2] = params[2];
		cntxt->lightAmbient[color].vec[3] = params[3];
		break;
	case NMGL_DIFFUSE:
		cntxt->lightDiffuse[color].vec[0] = params[0];
		cntxt->lightDiffuse[color].vec[1] = params[1];
		cntxt->lightDiffuse[color].vec[2] = params[2];
		cntxt->lightDiffuse[color].vec[3] = params[3];
		break;
	case NMGL_SPECULAR:
		cntxt->lightSpecular[color].vec[0] = params[0];
		cntxt->lightSpecular[color].vec[1] = params[1];
		cntxt->lightSpecular[color].vec[2] = params[2];
		cntxt->lightSpecular[color].vec[3] = params[3];
		break;
	case NMGL_POSITION:
		cntxt->tmp.vec[0] = params[0];
		cntxt->tmp.vec[1] = params[1];
		cntxt->tmp.vec[2] = params[2];
		cntxt->tmp.vec[3] = params[3];
		mul_mat4nm32f_v4nm32f(cntxt->modelviewMatrixStack.top(), 
			&cntxt->tmp, 
			&cntxt->lightPosition[color], 1);
		break;
	case NMGL_SPOT_DIRECTION:
		cntxt->lightSpotDirection[color].vec[0] = params[0];
		cntxt->lightSpotDirection[color].vec[1] = params[1];
		cntxt->lightSpotDirection[color].vec[2] = params[2];
		cntxt->lightSpotDirection[color].vec[3] = 0;
		break;
	case NMGL_SPOT_EXPONENT:
		if (params[0] < 0 || params[0]>128) {
			cntxt->error = NMGL_INVALID_VALUE;
			break;
		}
		else {
			cntxt->lightSpotExp[color] = params[0];
		}
		break;
	case NMGL_SPOT_CUTOFF:
		if ((params[0] < 0 || params[0]>90) && params[0] != 180) {
			cntxt->error = NMGL_INVALID_VALUE;
			break;
		}
		else {
			cntxt->lightSpotCutoff[color] = params[0];
		}
		break;
	case NMGL_CONSTANT_ATTENUATION:
		cntxt->lightConstAtt[color] = params[0];
		break;
	case NMGL_LINEAR_ATTENUATION:
		cntxt->lightLinAtt[color] = params[0];
		break;
	case NMGL_QUADRATIC_ATTENUATION:
		cntxt->lightQuadAtt[color] = params[0];
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		break;
	}
	
}
