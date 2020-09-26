#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglMultiTexCoord2f (NMGLenum target, NMGLfloat s, NMGLfloat t)
{
//	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	unsigned int tex_index = 0;

	if ((target < NMGL_TEXTURE0) || (target >= NMGL_TEXTURE0 + NMGL_MAX_TEX_UNITS))
	{
		cntxt.error = NMGL_INVALID_ENUM;
		return;
	}

	tex_index = target - NMGL_TEXTURE0;

	cntxt.texState.curTexCoords[tex_index].s = s;
	cntxt.texState.curTexCoords[tex_index].t = t;
	
}