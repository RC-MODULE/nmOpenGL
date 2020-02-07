#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"
#include "nmgl_data0.h"
#include "stdio.h"
#include "math.h"

//Функция рассчета освещения
//n - нормаль
//V - вершины
//^ - единичный вектор

extern "C" void maskEq0(v4nm32f* srcVec, v2nm32f* srcMask, v4nm32f* dstVec, int size);

SECTION(".text_demo3d")
void light(v4nm32f* vertex, v4nm32f* srcNormalDstColor, int countVertex) {
	v2nm32f* n_dot_vp = (v2nm32f*)cntxt.buffer0;
	v2nm32f* n_dot_direct_h = (v2nm32f*)cntxt.buffer3;
	v4nm32f* h = (v4nm32f*)cntxt.buffer2;
	v4nm32f* subVP = (v4nm32f*)cntxt.buffer1;

	if (cntxt.lightPosition[0].vec[3] == 0) {
		set_v4nm32f((v4nm32f*)cntxt.buffer0, &cntxt.lightPosition[0], countVertex);
	}
	else {
		subCRev_v4nm32f(vertex, (v4nm32f*)&cntxt.lightPosition[0], (v4nm32f*)cntxt.buffer0, countVertex);
	}
	dotV_gt0_v4nm32f((v4nm32f*)cntxt.buffer0, (v4nm32f*)cntxt.buffer0, (v2nm32f*)cntxt.buffer1, countVertex);
	fastInvSqrt(cntxt.buffer1, cntxt.buffer2, 2 * countVertex);
	dotMulV_v4nm32f((v2nm32f*)cntxt.buffer2, (v4nm32f*)cntxt.buffer0, subVP, countVertex);

	dotV_gt0_v4nm32f(srcNormalDstColor, subVP, n_dot_vp, countVertex);
	// VP - cntxt.buffer1(12*size)
	// n*VP - cntxt.buffer0(6*size)

	cntxt.tmp.vec[0] = cntxt.tmp.vec[1] = cntxt.tmp.vec[3] = 0;
	cntxt.tmp.vec[2] = 1;
	addC_v4nm32f((v4nm32f*)subVP, &cntxt.tmp, (v4nm32f*)h, countVertex);
	// VP - free
	// h - cntxt.buffer2 (12*size)

	dotV_gt0_v4nm32f((v4nm32f*)h, (v4nm32f*)h, (v2nm32f*)cntxt.buffer3, countVertex);			// h*h
	fastInvSqrt(cntxt.buffer3, (cntxt.buffer0 + 6 * NMGL_SIZE), 2 * countVertex);
	dotMulV_v4nm32f((v2nm32f*)(cntxt.buffer0 + 6 * NMGL_SIZE), h, (v4nm32f*)cntxt.buffer1, countVertex);

	dotV_gt0_v4nm32f((v4nm32f*)srcNormalDstColor, (v4nm32f*)cntxt.buffer1, n_dot_direct_h, countVertex);
	// h - free
	// n*^h -cntxt.buffer3 (6*size)

	//чтобы ноль тоже возводился в степень и получась единица, заменяем 0 на 1
	replaceEq0_32f((float*)n_dot_direct_h, (float*)n_dot_direct_h, 2 * countVertex, 1.0f);							
	pow_32f((float*)n_dot_direct_h, cntxt.buffer2, cntxt.specularExp, 2 * countVertex, (float*)n_dot_direct_h);			//(n*^h)^srm
	dotMulC_AddC_v4nm32f((v2nm32f*)cntxt.buffer2, &cntxt.specularMul[0], &cntxt.ambientMul[0], (v4nm32f*)cntxt.buffer3, countVertex);		//* scm*scli
	maskEq0((v4nm32f*)cntxt.buffer3, (v2nm32f*)n_dot_vp, (v4nm32f*)cntxt.buffer2, countVertex);			//f
	dotMulC_Add_v4nm32f(n_dot_vp, &cntxt.diffuseMul[0], (v4nm32f*)cntxt.buffer2, (v4nm32f*)srcNormalDstColor, countVertex);
	// n*VP - free
	// n*^h - free	
}