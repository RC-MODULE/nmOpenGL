#include "demo3d_nm0.h"
#include "nmgl.h"

#include <math.h>

#pragma code_section ".text_nmgl"


extern "C" void mul_mat4nm32f_v4nm32f(mat4nm32f* srcMatrix4x4, v4nm32f* srcVector4xN, v4nm32f* dstVector4xN, int count);

SECTION(".text_nmgl")
void nmglFrustumf (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar){
	mat4nm32f temp ={2*zNear/(right-left),0,0,0,
					 0,2*zNear/(top-bottom),0,0,
		(right + left) / (right - left),(top + bottom) / (top - bottom),-(zFar+zNear)/(zFar-zNear), -1,
					 0,0,-2 * zFar*zNear / (zFar - zNear),0};
	
	NMGL_Context *cntxt = NMGL_GetCurrentContext();
	
	mat4nm32f* current = cntxt->currentMatrixStack->top();
	mul_mat4nm32f_v4nm32f(current, (v4nm32f*)&temp, (v4nm32f*)current, 4);

	if (cntxt->currentMatrixStack->type == NMGL_MODELVIEW_MATRIX) {
		reverseMatrix3x3in4x4(cntxt->modelviewMatrixStack.top(), &cntxt->normalMatrix);
	}
}
