#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")

void nmglGetMaterialfv (NMGLenum face, NMGLenum pname, NMGLfloat *params)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	if((face != NMGL_FRONT) && (face != NMGL_BACK)){
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	switch(pname){
	case NMGL_AMBIENT:
		for (int i = 0; i < 4; i++) {
			params[i] = (*cntxt->lightingInfo.pMaterialAmbient).vec[i];
		}
		break;
	case NMGL_DIFFUSE:
		for (int i = 0; i < 4; i++) {
			params[i] = (*cntxt->lightingInfo.pMaterialDiffuse).vec[i];
		}
		break;
	case NMGL_SPECULAR:
		for (int i = 0; i < 4; i++) {
			params[i] = cntxt->lightingInfo.materialSpecular.vec[i];
		}
		break;
	case NMGL_EMISSION:
		for (int i = 0; i < 4; i++) {
			params[i] = cntxt->lightingInfo.materialEmissive.vec[i];
		}
		break;
	case NMGL_SHININESS:
		params[0] = cntxt->lightingInfo.specularExp;
		break;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
	}
}
