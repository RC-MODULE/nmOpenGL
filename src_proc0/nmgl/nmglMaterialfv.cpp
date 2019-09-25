#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglMaterialfv(NMGLenum face, NMGLenum pname, const NMGLfloat *params) {
	if(face != NMGL_FRONT_AND_BACK){
		cntxt.error = NMGL_INVALID_ENUM;
		return;
	}
	switch (pname)
	{
	case NMGL_AMBIENT:
	case NMGL_AMBIENT_AND_DIFFUSE:
		cntxt.materialAmbient.vec[0] = params[0];
		cntxt.materialAmbient.vec[1] = params[1];
		cntxt.materialAmbient.vec[2] = params[2];
		cntxt.materialAmbient.vec[3] = params[3];
		if(pname==NMGL_AMBIENT){
			break;
		}
	case NMGL_DIFFUSE:
		cntxt.materialDiffuse.vec[0] = params[0];
		cntxt.materialDiffuse.vec[1] = params[1];
		cntxt.materialDiffuse.vec[2] = params[2];
		cntxt.materialDiffuse.vec[3] = params[3];
		break;
	case NMGL_SPECULAR:
		cntxt.materialSpecular.vec[0] = params[0];
		cntxt.materialSpecular.vec[1] = params[1];
		cntxt.materialSpecular.vec[2] = params[2];
		cntxt.materialSpecular.vec[3] = params[3];
		break;
	case NMGL_EMISSION:
		cntxt.materialEmissive.vec[0] = params[0];
		cntxt.materialEmissive.vec[1] = params[1];
		cntxt.materialEmissive.vec[2] = params[2];
		cntxt.materialEmissive.vec[3] = params[3];
		break;
	case NMGL_SHININESS:
		if (params[0] < 0 || params[0]>128) {
			cntxt.error = NMGL_INVALID_VALUE;
			break;
		}
		else {
			cntxt.specularExp = params[0];
		}
		break;
	default:
		cntxt.error = NMGL_INVALID_ENUM;
		break;
	}
	
}