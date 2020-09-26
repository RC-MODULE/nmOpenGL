#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglTexParameteri (NMGLenum target, NMGLenum pname, NMGLint param)
{
//	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	
	if (target != NMGL_TEXTURE_2D)
	{
		cntxt.error = NMGL_INVALID_ENUM;
		return;
	}

	if (pname == NMGL_TEXTURE_WRAP_S) 
	{
		switch (param)
		{
			case NMGL_REPEAT:
			case NMGL_CLAMP_TO_EDGE: cntxt.texState.texUnits[cntxt.texState.activeTexUnitIndex].boundTexObject->texWrapS = param;
									 break;
			default: 				 cntxt.error = NMGL_INVALID_VALUE;
					 	 	 		 return;
		}
	}
	else if (pname == NMGL_TEXTURE_WRAP_T) 
	{
		switch (param)
		{
			case NMGL_REPEAT:
			case NMGL_CLAMP_TO_EDGE: cntxt.texState.texUnits[cntxt.texState.activeTexUnitIndex].boundTexObject->texWrapT = param;
									 break;
			default: 				 cntxt.error = NMGL_INVALID_VALUE;
					 	 	 		 return;
		}
	}
	else if (pname == NMGL_TEXTURE_MIN_FILTER)
	{
		switch (param)
		{
			case NMGL_NEAREST:
			case NMGL_LINEAR:
			case NMGL_NEAREST_MIPMAP_NEAREST:
			case NMGL_NEAREST_MIPMAP_LINEAR:
			case NMGL_LINEAR_MIPMAP_NEAREST:
			case NMGL_LINEAR_MIPMAP_LINEAR:	cntxt.texState.texUnits[cntxt.texState.activeTexUnitIndex].boundTexObject->texMinFilter = param;
									 	 	break;
			default: 				 		cntxt.error = NMGL_INVALID_VALUE;
					 	 	 		 	 	return;
		}
	}
	else if (pname == NMGL_TEXTURE_MAG_FILTER)
	{
		switch (param)
		{
			case NMGL_NEAREST:
			case NMGL_LINEAR: cntxt.texState.texUnits[cntxt.texState.activeTexUnitIndex].boundTexObject->texMagFilter = param;
									 break;
			default: 		  cntxt.error = NMGL_INVALID_VALUE;
					 	 	  return;
		}
	}
	else
	{
		cntxt.error = NMGL_INVALID_ENUM;
		return;
	}

	cntxt.synchro.writeInstr(1, NMC1_SET_TEX_PARAMI, (unsigned int)pname, (unsigned int)param);
	
}