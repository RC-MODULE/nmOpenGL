#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglMultiTexCoord2fv (NMGLenum target, const NMGLfloat *v)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	unsigned int tex_index = 0;

	if ((target < NMGL_TEXTURE0) || (target >= NMGL_TEXTURE0 + NMGL_MAX_TEX_UNITS))
	{
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	if (v == NULL)
	{
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}

	tex_index = target - NMGL_TEXTURE0;

	cntxt->texState.curTexCoords[tex_index].s = v[0];
	cntxt->texState.curTexCoords[tex_index].t = v[1];
	
}