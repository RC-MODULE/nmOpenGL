/*!
 *  \file light.cpp
 *  \author Zhilenkov Ivan
 */

#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl.h"

#include "stdio.h"
#include "math.h"

//функция рассчета освещени€
//n - нормаль
//V - вершины
//^ - единичный вектор

//SECTION(".data_imu3") v4nm32f resultColor[3 * NMGL_SIZE];

extern "C" void maskEq0(v4nm32f* srcVec, v2nm32f* srcMask, v4nm32f* dstVec, int size);


inline void normalize_v4nm32f(v4nm32f* src, v4nm32f* dst, int size, v2nm32f* tmp1, v2nm32f* tmp2) {
	dotV_gt0_v4nm32f(src, src, tmp1, size);	
	fastInvSqrt((float*)tmp1, (float*)tmp2, 2 * size);
	dotMulV_v4nm32f(tmp2, src, dst, size);
}



SECTION(".text_demo3d")
void light(v4nm32f* vertex, v4nm32f* srcNormalDstColor, int countVertex) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	v4nm32f *resultColor = (v4nm32f*)cntxt->buffer4;
	v2nm32f *n_dot_vp = (v2nm32f*)(cntxt->buffer0 + 6 * NMGL_SIZE);
	v2nm32f *n_dot_h_in_srm = (v2nm32f*)(cntxt->buffer1 + 6 * NMGL_SIZE);
	v2nm32f *length_vp = (v2nm32f*)(cntxt->buffer2 + 6 * NMGL_SIZE);
	v2nm32f *sqr_length_vp = (v2nm32f*)(cntxt->buffer3 + 6 * NMGL_SIZE);

	cntxt->tmp.vec[0] = cntxt->tmp.vec[1] = cntxt->tmp.vec[2] = cntxt->tmp.vec[3] = 0;
	set_v4nm32f(resultColor, &cntxt->tmp, countVertex);

	for (int light = 0; light < MAX_LIGHTS; light++) {
		if (!cntxt->isEnabledLight[light]) {
			continue;
		}

		if (cntxt->lightPosition[light].vec[3] == 0) {
			set_v4nm32f((v4nm32f*)cntxt->buffer1, cntxt->lightPosition + light, countVertex);
		}
		else {
			subCRev_v4nm32f(vertex, cntxt->lightPosition + light, (v4nm32f*)cntxt->buffer1, countVertex);
		}
		normalize_v4nm32f((v4nm32f*)cntxt->buffer1, (v4nm32f*)cntxt->buffer2, countVertex,
			(v2nm32f*)cntxt->buffer0, (v2nm32f*)cntxt->buffer3);
		//cntxt->buffer2 = unit VP

		dotV_gt0_v4nm32f(srcNormalDstColor, (v4nm32f*)cntxt->buffer2, (v2nm32f*)n_dot_vp, countVertex);
		//printCrc(n_dot_vp, 2 * countVertex, "n_dot_vp");
		//printf("size=%d\n\n", countVertex);
		//printCrc(srcNormalDstColor, 4 * countVertex, "srcn");

		cntxt->tmp.vec[0] = cntxt->tmp.vec[1] = cntxt->tmp.vec[3] = 0;
		cntxt->tmp.vec[2] = 1;
		addC_v4nm32f((v4nm32f*)cntxt->buffer2, &cntxt->tmp, (v4nm32f*)cntxt->buffer1, countVertex);
		//cntxt->buffer1 = unit VP + h
		normalize_v4nm32f((v4nm32f*)cntxt->buffer1, (v4nm32f*)cntxt->buffer2, countVertex,
			(v2nm32f*)cntxt->buffer0, (v2nm32f*)cntxt->buffer3);
		//cntxt->buffer2 = unit(unit VP + h)
		dotV_gt0_v4nm32f(srcNormalDstColor, (v4nm32f*)cntxt->buffer2, (v2nm32f*)cntxt->buffer3, countVertex);
		//printCrc(srcNormalDstColor, 4 * countVertex, "src1");
		//printCrc(cntxt->buffer2, 4 * countVertex, "src2");
		//printCrc(cntxt->buffer3, 2 * countVertex, "dst");
		//printf("\n");
		replaceEq0_32f((float*)cntxt->buffer3, (float*)cntxt->buffer3, 2 * countVertex, 1.0f);
		pow_32f(cntxt->buffer3, (float*)n_dot_h_in_srm, cntxt->specularExp, 2 * countVertex, cntxt->buffer2);
		
		baseLighti(cntxt->ambientMul + light,
			n_dot_vp,
			cntxt->diffuseMul + light,
			n_dot_h_in_srm,
			cntxt->specularMul + light,
			(v4nm32f*)cntxt->buffer2,
			countVertex);

		nmppsAdd_32f(cntxt->buffer2, (float*)resultColor, (float*)resultColor, 4 * countVertex);
	}
	addC_v4nm32f((v4nm32f*)resultColor, &cntxt->ambientMul[MAX_LIGHTS],
		(v4nm32f*)srcNormalDstColor, countVertex);

	
}