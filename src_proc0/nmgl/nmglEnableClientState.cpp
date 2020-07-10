#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglEnableClientState(NMGLenum array) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	switch (array)
	{
	case NMGL_VERTEX_ARRAY:
		cntxt->vertexArray.enabled = NMGL_TRUE;
		break;
	case NMGL_COLOR_ARRAY:
		cntxt->colorArray.enabled = NMGL_TRUE;
		break;
	case NMGL_NORMAL_ARRAY:
		cntxt->normalArray.enabled = NMGL_TRUE;
	default:
		cntxt->error = NMGL_INVALID_ENUM;
		break;
	}
}