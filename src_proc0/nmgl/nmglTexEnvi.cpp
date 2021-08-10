#include "demo3d_nm0.h"
#include "nmgl.h"


#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglTexEnvi (NMGLenum target, NMGLenum pname, NMGLint param)
{
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	if ((target != NMGL_TEXTURE_ENV) || (pname != NMGL_TEXTURE_ENV_MODE))
	{
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	switch (param)
	{
		case NMGL_REPLACE:
		case NMGL_DECAL:
		case NMGL_BLEND:
		case NMGL_ADD:
		case NMGL_MODULATE: cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].texFunctionName = param;
							break;
		//TODO: возможно, данная функция также устанавливает указатель на функцию текстурирования.

		default: 			cntxt->error = NMGL_INVALID_VALUE;
				 	 	 	return;
	}

	NM_Command command;
	command.instr = NMC1_SET_TEX_ENV_MODE;
	command.params[0] = CommandArgument(param);
	cntxt->synchro.pushInstr(&command);
	//cntxt->synchro.writeInstr(1, NMC1_SET_TEX_ENV_MODE, (unsigned int)param);
	
}