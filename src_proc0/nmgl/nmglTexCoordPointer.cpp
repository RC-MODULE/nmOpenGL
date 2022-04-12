#include "demo3d_nm0.h"
#include "nmgl.h"
//#include "nmgl_data0.h"
#include "debugprint.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglTexCoordPointer (NMGLint size, NMGLenum type, NMGLsizei stride, const void *pointer)
{
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	if ((size != 2) || (stride != 0) || (pointer == NULL))
	{
		cntxt->error = NMGL_INVALID_VALUE;
		return;
	}

	if (type != NMGL_FLOAT)
	{
		cntxt->error = NMGL_INVALID_ENUM;
		return;
	}

	cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnitIndex].size = size;
	cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnitIndex].type = type;
	cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnitIndex].stride = stride;
	cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnitIndex].pointer = pointer;
	
}
