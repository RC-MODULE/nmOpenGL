#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include <math.h>

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglOrthof (NMGLfloat left, NMGLfloat right, NMGLfloat bottom, NMGLfloat top, NMGLfloat zNear, NMGLfloat zFar){
	mat4nm32f temp ={2/(right-left),0,0,0,
					 0,2/(top-bottom),0,0,
					 0,0,2/(zFar-zNear),0,
		-(right + left) / (right - left),-(top + bottom) / (top - bottom),-(zFar + zNear) / (zFar - zNear),1};
	
	mat4nm32f* current = cntxt.currentMatrixStack->top();
	mul_mat4nm32f_v4nm32f(current, (v4nm32f*)&temp, (v4nm32f*)current, 4);
}
