/*!
 *  \file copyColorByIndices_BGRA_RGBA.cpp
 *  \author Жиленков Иван
 */
#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmtype.h"

SECTION(".data_imu4")	v4nm32f colorValues[3 * NMGL_SIZE];

SECTION(".text_demo3d")
void startCalculateColor(v4nm32f* srcVertex, v4nm32f* srcNormal, v4nm32f* srcColor, int vertexCount) {
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	nmblas_scopy(vertexCount * 4, (float*)srcNormal, 1, (float*)colorValues, 1);
	if(cntxt->isLighting){
		light(srcVertex, colorValues, vertexCount);
	}
	else {
		cntxt->tmp.vec[0] = 1;
		cntxt->tmp.vec[1] = 1;
		cntxt->tmp.vec[2] = 1;
		cntxt->tmp.vec[3] = 1;
		set_v4nm32f(colorValues, &cntxt->tmp, vertexCount);
	}
	if (cntxt->colorArray.enabled) {
		if (cntxt->colorArray.type == NMGL_UNSIGNED_BYTE) {
			nmppsConvert_32s32f((int*)srcColor, cntxt->buffer0, cntxt->colorArray.size * vertexCount);
			nmppsMulC_32f(cntxt->buffer0, (float*)colorValues, 1.0 / 255.0, cntxt->colorArray.size * vertexCount);
		}
	}
	clamp_32f((float*)colorValues, 0, 1, (float*)cntxt->buffer3, 4 * vertexCount);
	cntxt->tmp.vec[0] = RED_COEFF;
	cntxt->tmp.vec[1] = GREEN_COEFF;
	cntxt->tmp.vec[2] = BLUE_COEFF;
	cntxt->tmp.vec[3] = ALPHA_COEFF;

	mulC_v4nm32f((v4nm32f*)cntxt->buffer3, &cntxt->tmp, colorValues, vertexCount);
}

SECTION(".text_demo3d")
void getCalculatedColor(v4nm32f* dstVertex, int vertexCount) {
	nmblas_scopy(vertexCount * 4, (float*)colorValues, 1, (float*)dstVertex, 1);
}
