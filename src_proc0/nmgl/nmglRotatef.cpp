#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include <math.h>

#pragma code_section ".text_nmgl"

SECTION(".text_nmgl")
void nmglRotatef(NMGLfloat angle, NMGLfloat x, NMGLfloat y, NMGLfloat z) {
	mat4nm32f temp ={1,0,0,0,
					 0,1,0,0,
					 0,0,1,0,
					 0,0,0,1};
	float cos;
	float sin;

	float rad = angle*3.1415926 / 180;


	//nmppsCos_32f(&angle, &cos, 1);
	//nmppsSin_32f(&angle, &sin, 1);
	cos = cosf(rad);
	sin = sinf(rad);
	
	temp.matr[0*4+0] = (cos + x*x*(1-cos));
	temp.matr[0*4+1] = ((1-cos)*y*x + z*sin);
	temp.matr[0*4+2] = ((1-cos)*z*x - y*sin);
	
	///
	temp.matr[1*4+0] = ((1-cos)*x*y - z*sin);
	temp.matr[1*4+1] = (cos + y*y*(1-cos)); 
	temp.matr[1*4+2] = ((1-cos)*z*y + x*sin);
	
	///  
	temp.matr[2*4+0] = ((1-cos)*x*z + y*sin);
	temp.matr[2*4+1] = ((1-cos)*y*z - x*sin);
	temp.matr[2*4+2] = (cos + z*z*(1-cos));

	mat4nm32f* current = cntxt.currentMatrixStack->top();
	mul_mat4nm32f_v4nm32f(current, (v4nm32f*)&temp, (v4nm32f*)current, 4);
	
}
