#include "demo3d_nm0.h"
#include "nmgl.h"
#include "debugprint.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglClientActiveTexture (NMGLenum texture)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	if ((texture < NMGL_TEXTURE0) || (texture >= NMGL_TEXTURE0 + NMGL_MAX_TEX_UNITS))
	{
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	cntxt->texState.clientActiveTexUnit = texture;
	cntxt->texState.clientActiveTexUnitIndex = texture - NMGL_TEXTURE0;
}