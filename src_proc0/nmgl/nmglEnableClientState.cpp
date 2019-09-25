#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglEnableClientState(NMGLenum array) {
	if (array == NMGL_VERTEX_ARRAY) {
		cntxt.vertexArray.enabled = NMGL_TRUE;
	}
	if (array == NMGL_COLOR_ARRAY) {
		cntxt.colorArray.enabled = NMGL_TRUE;
	}
	if (array == NMGL_NORMAL_ARRAY) {
		cntxt.normalArray.enabled = NMGL_TRUE;
	}
}