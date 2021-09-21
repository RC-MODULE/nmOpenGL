#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmglservice_nm0.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglActiveTexture (NMGLenum texture)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	if ((texture < NMGL_TEXTURE0) || (texture >= NMGL_TEXTURE0 + NMGL_MAX_TEX_UNITS))
	{
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	cntxt->texState.activeTexUnit = texture;
	
	unsigned int activeTexUnitIndex = texture - NMGL_TEXTURE0;
	cntxt->texState.activeTexUnitIndex = activeTexUnitIndex;

	NM_Command command;
	command.instr = NMC1_SET_ACTIVE_TEXTURE;
	command.params[0] = CommandArgument(activeTexUnitIndex);
	//cntxt->synchro.pushInstr(&command);	
	NMGL_SetValue(command);
}
