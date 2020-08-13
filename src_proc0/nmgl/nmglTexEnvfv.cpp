#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglTexEnvfv (NMGLenum target, NMGLenum pname, const NMGLfloat *params)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	int i = 0;

	if ((target != NMGL_TEXTURE_ENV) || (pname != NMGL_TEXTURE_ENV_COLOR))
	{
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	if (params == NULL)
	{
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}

	for (i = 0; i < 4; i++)
	{
		if (params[i] < 0.0f)
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[i] = 0.0f;
		else if (params[i] > 1.0f)
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[i] = 1.0f;
		else
			cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[i] = params[i];
 	}
	
	unsigned int texEnvColorInt [4];
	for (i = 0; i < 4; i++)
	{
		texEnvColorInt[i] = 255 * cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texEnvColor[i];
 	}
	cntxt->synchro.writeInstr(1, NMC1_SET_TEX_ENV_COLOR, texEnvColorInt[0], texEnvColorInt[1], texEnvColorInt[2], texEnvColorInt[3]);
	
}