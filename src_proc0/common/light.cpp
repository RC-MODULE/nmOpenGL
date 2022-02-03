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
#include "utility_float.h"


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
void light(v4nm32f* vertex, v4nm32f* srcNormal, v4nm32f* srcDstColor, int countVertex) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	LightingInfo *lightingInfo = &cntxt->lightingInfo;
	v4nm32f *resultColor = (v4nm32f*)cntxt->buffer4;
	v2nm32f *n_dot_vp = (v2nm32f*)(cntxt->buffer0 + 6 * NMGL_SIZE);
	v2nm32f *n_dot_h_in_srm = (v2nm32f*)(cntxt->buffer1 + 6 * NMGL_SIZE);
	v2nm32f *length_vp = (v2nm32f*)(cntxt->buffer2 + 6 * NMGL_SIZE);
	v2nm32f *sqr_length_vp = (v2nm32f*)(cntxt->buffer3 + 6 * NMGL_SIZE);

	cntxt->tmp.vec[0] = cntxt->tmp.vec[1] = cntxt->tmp.vec[2] = cntxt->tmp.vec[3] = 0;
	set_v4nm32f(resultColor, &cntxt->tmp, countVertex);

	for (int light = 0; light < MAX_LIGHTS; light++) {
		if (!lightingInfo->isEnabledLight[light]) {
			continue;
		}

		if (lightingInfo->lightPosition[light].vec[3] == 0) {
			set_v4nm32f((v4nm32f*)cntxt->buffer1, lightingInfo->lightPosition + light, countVertex);
		}
		else {
			subCRev_v4nm32f(vertex, lightingInfo->lightPosition + light, (v4nm32f*)cntxt->buffer1, countVertex);
		}
		normalize_v4nm32f((v4nm32f*)cntxt->buffer1, (v4nm32f*)cntxt->buffer2, countVertex,
			(v2nm32f*)cntxt->buffer0, (v2nm32f*)cntxt->buffer3);
		//cntxt->buffer2 = unit VP

		dotV_gt0_v4nm32f(srcNormal, (v4nm32f*)cntxt->buffer2, (v2nm32f*)n_dot_vp, countVertex);
		//printCrc(n_dot_vp, 2 * countVertex, "n_dot_vp");
		//printf("size=%d\n\n", countVertex);
		//printCrc(srcNormal, 4 * countVertex, "srcn");

		cntxt->tmp.vec[0] = cntxt->tmp.vec[1] = cntxt->tmp.vec[3] = 0;
		cntxt->tmp.vec[2] = 1;
		addC_v4nm32f((v4nm32f*)cntxt->buffer2, &cntxt->tmp, (v4nm32f*)cntxt->buffer1, countVertex);
		//cntxt->buffer1 = unit VP + h
		normalize_v4nm32f((v4nm32f*)cntxt->buffer1, (v4nm32f*)cntxt->buffer2, countVertex,
			(v2nm32f*)cntxt->buffer0, (v2nm32f*)cntxt->buffer3);
		//cntxt->buffer2 = unit(unit VP + h)
		dotV_gt0_v4nm32f(srcNormal, (v4nm32f*)cntxt->buffer2, (v2nm32f*)cntxt->buffer3, countVertex);
		//printCrc(srcNormalDstColor, 4 * countVertex, "src1");
		//printCrc(cntxt->buffer2, 4 * countVertex, "src2");
		//printCrc(cntxt->buffer3, 2 * countVertex, "dst");
		//printf("\n");
		replaceEq0_32f((float*)cntxt->buffer3, (float*)cntxt->buffer3, 2 * countVertex, 1.0f);
		pow_32f(cntxt->buffer3, (float*)n_dot_h_in_srm, lightingInfo->specularExp, 2 * countVertex, cntxt->buffer2);
		
		if (lightingInfo->isColorMaterial == 0) {
			baseLighti(lightingInfo->ambientMul + light,
				n_dot_vp,
				lightingInfo->diffuseMul + light,
				n_dot_h_in_srm,
				lightingInfo->specularMul + light,
				(v4nm32f*)cntxt->buffer2,
				countVertex);
		}
		else {
			v4nm32f *diffuse = (v4nm32f*)(cntxt->buffer2);
			v4nm32f *ambient = (v4nm32f*)(cntxt->buffer5);

			mulC_v4nm32f(srcDstColor, lightingInfo->lightDiffuse + light, diffuse, countVertex);
			mulC_v4nm32f(srcDstColor, lightingInfo->lightAmbient + light, ambient, countVertex);

			// ambient + n_dot_vp * diffuse + n_dot_h_in_srm * specular
			// n_dot_vp * diffuse
			dotMulV_v4nm32f(n_dot_vp, diffuse, (v4nm32f *)cntxt->buffer3, countVertex);
			//ambient + n_dot_vp * diffuse
			nmppsAdd_32f((float*)cntxt->buffer3, (float*)ambient, (float*)cntxt->buffer2, 4 * countVertex);
			//n_dot_h_in_srm * specular
			//void dotMulC_Add_v4nm32f(v2nm32f* srcVec, v4nm32f* mulC, v4nm32f* addVec, v4nm32f* dst, int size);
			dotMulC_Add_v4nm32f(n_dot_h_in_srm, lightingInfo->specularMul + light, (v4nm32f*)cntxt->buffer2, (v4nm32f*)cntxt->buffer3, countVertex);

			nmblas_scopy(4 * countVertex, cntxt->buffer3, 1, cntxt->buffer2, 1);
			
		}

		nmppsAdd_32f(cntxt->buffer2, (float*)resultColor, (float*)resultColor, 4 * countVertex);
	}

	if (lightingInfo->isColorMaterial == 0) {
		addC_v4nm32f((v4nm32f*)resultColor, &lightingInfo->ambientMul[MAX_LIGHTS],
					 (v4nm32f*)srcDstColor, countVertex);
	} else {
		v4nm32f *acm_mul_acs = (v4nm32f*)(cntxt->buffer2);
		v4nm32f *acm_mul_acs_plus_ecm = (v4nm32f*)(cntxt->buffer3);

		// resultColor + acm * acs + ecm
		// acm * acs; acm = vertex colors (COLOR_MATERIAL enabled), acs = lightAmbient[MAX_LIGHTS]
		mulC_v4nm32f(srcDstColor, &lightingInfo->lightAmbient[MAX_LIGHTS], acm_mul_acs, countVertex);
		// acm * acs + ecm
		addC_v4nm32f(acm_mul_acs, &lightingInfo->materialEmissive, acm_mul_acs_plus_ecm, countVertex);

		nmppsAdd_32f((float*)acm_mul_acs_plus_ecm, (float*)resultColor, (float*)srcDstColor, 4 * countVertex);
	}



}
