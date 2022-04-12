#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglGetTexEnviv (NMGLenum env, NMGLenum pname, NMGLint *params)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	if ((env != NMGL_TEXTURE_ENV) || (pname != NMGL_TEXTURE_ENV_MODE))
	{
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	if (params == NULL)
	{
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}

	params[0] = cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName;
	
}