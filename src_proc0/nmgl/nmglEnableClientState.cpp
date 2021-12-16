#include "demo3d_nm0.h"
#include "nmgl.h"

#include "stdio.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglEnableClientState(NMGLenum array) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	NMGL_Context *context = NMGL_GetCurrentContext();

	switch (array)
	{
	case NMGL_VERTEX_ARRAY:
		context->vertexArray.enabled = NMGL_TRUE;
		break;
	case NMGL_COLOR_ARRAY:
		context->colorArray.enabled = NMGL_TRUE;
		break;
	case NMGL_NORMAL_ARRAY:
		context->normalArray.enabled = NMGL_TRUE;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		break;
	}
	if (array == NMGL_TEXTURE_COORD_ARRAY) {
		cntxt->texState.texcoordArray[cntxt->texState.clientActiveTexUnitIndex].enabled = NMGL_TRUE;
	}
}