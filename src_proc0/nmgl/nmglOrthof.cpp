#include "demo3d_nm0.h"
#include "nmgl.h"
#include "utility_float.h"
#include "debugprint.h"

#include <math.h>

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglOrthof (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar){
	DEBUG_PLOG_FUNCTION();
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	mat4nm32f temp ={2/(right-left),0,0,0,
					 0,2/(top-bottom),0,0,
					 0,0,-2/(zFar-zNear),0,
		-(right + left) / (right - left),-(top + bottom) / (top - bottom),-(zFar + zNear) / (zFar - zNear),1};
	
	mat4nm32f* current = cntxt->currentMatrixStack->top();
	mul_mat4nm32f_v4nm32f(current, (v4nm32f*)&temp, (v4nm32f*)current, 4);

	if (cntxt->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
		reverseMatrix3x3in4x4(cntxt->modelviewMatrixStack.top(), &cntxt->normalMatrix);
	}
}
