#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmgl.h"

#include <math.h>

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglScalef(NMGLfloat scaleX, NMGLfloat scaleY, NMGLfloat scaleZ) {
	mat4nm32f temp ={scaleX,0,0,0,
					 0,scaleY,0,0,
					 0,0,scaleZ,0,
					 0,0,0,1};
	
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	mat4nm32f* current = cntxt->currentMatrixStack->top();
	mul_mat4nm32f_v4nm32f(current, (v4nm32f*)&temp, (v4nm32f*)current, 4);

	if (cntxt->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
		reverseMatrix3x3in4x4(cntxt->modelviewMatrixStack.top(), &cntxt->normalMatrix);
	}
}
