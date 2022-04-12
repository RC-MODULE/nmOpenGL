#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglGetTexParameteriv (NMGLenum target, NMGLenum pname, NMGLint *params)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	if (target != NMGL_TEXTURE_2D){
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	if (params == NULL){
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}
	
	if (pname == NMGL_TEXTURE_WRAP_S){
		params[0] = cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texWrapS;
	}
	else if (pname == NMGL_TEXTURE_WRAP_T){
		params[0] = cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texWrapT;
	}
	else if (pname == NMGL_TEXTURE_MIN_FILTER){
		params[0] = cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texMinFilter;
	}
	else if (pname == NMGL_TEXTURE_MAG_FILTER){
		params[0] = cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject->texMagFilter;
	}
	else{
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}
	
}