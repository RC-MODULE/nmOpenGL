#include "demo3d_nm0.h"
#include "nmgl.h"
#include "lighting.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglMaterialfv(NMGLenum face, NMGLenum pname, const NMGLfloat *params) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	LightingInfo *lightingInfo = &NMGL_GetCurrentContext()->lightingInfo;

	if(face != NMGL_FRONT_AND_BACK){
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	switch (pname)
	{
	case NMGL_AMBIENT:
	case NMGL_AMBIENT_AND_DIFFUSE:
		lightingInfo->materialAmbient.vec[0] = params[0];
		lightingInfo->materialAmbient.vec[1] = params[1];
		lightingInfo->materialAmbient.vec[2] = params[2];
		lightingInfo->materialAmbient.vec[3] = params[3];
		if(pname==NMGL_AMBIENT){
			break;
		}
	case NMGL_DIFFUSE:
		lightingInfo->materialDiffuse.vec[0] = params[0];
		lightingInfo->materialDiffuse.vec[1] = params[1];
		lightingInfo->materialDiffuse.vec[2] = params[2];
		lightingInfo->materialDiffuse.vec[3] = params[3];
		break;
	case NMGL_SPECULAR:
		lightingInfo->materialSpecular.vec[0] = params[0];
		lightingInfo->materialSpecular.vec[1] = params[1];
		lightingInfo->materialSpecular.vec[2] = params[2];
		lightingInfo->materialSpecular.vec[3] = params[3];
		break;
	case NMGL_EMISSION:
		lightingInfo->materialEmissive.vec[0] = params[0];
		lightingInfo->materialEmissive.vec[1] = params[1];
		lightingInfo->materialEmissive.vec[2] = params[2];
		lightingInfo->materialEmissive.vec[3] = params[3];
		break;
	case NMGL_SHININESS:
		if (params[0] < 0 || params[0]>128) {
			cntxt->error = NMGL_INVALID_VALUE;
			break;
		}
		else {
			lightingInfo->specularExp = params[0];
		}
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		break;
	}
	
}