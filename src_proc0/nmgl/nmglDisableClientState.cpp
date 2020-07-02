#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglDisableClientState(NMGLenum array) {
	if (array == NMGL_VERTEX_ARRAY) {
		cntxt.vertexArray.enabled = NMGL_FALSE;
	}
	if (array == NMGL_COLOR_ARRAY) {
		cntxt.colorArray.enabled = NMGL_FALSE;
	}
	if (array == NMGL_NORMAL_ARRAY) {
		cntxt.normalArray.enabled = NMGL_FALSE;
	}
	if (array == NMGL_TEXTURE_COORD_ARRAY) {
		cntxt.texState.texcoordArray[cntxt.texState.clientActiveTexUnitIndex].enabled = NMGL_FALSE;
	}
}