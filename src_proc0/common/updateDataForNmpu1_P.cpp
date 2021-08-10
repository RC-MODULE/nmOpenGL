/*!
 *  \file updateDataForNmpu1_P.cpp
 *  \author Zhilenkov Ivan
 */
#include "demo3d_nm0.h"
#include "nmpp.h"


#include "stdio.h"
#include "nmblas.h"

SECTION(".text_demo3d")
//void updateDataForNmpu1_T(DataForNmpu1* data, Triangles* triangles, int count, int segX, int segY){
void updatePolygonsP(DataForNmpu1* data, Points* points, int count, v2nm32f lowerLeft){
	NMGL_Context_NM0* cntxt = NMGL_Context_NM0::getContext();
	float* temp0 = cntxt->buffer0 + 4 * NMGL_SIZE;
	float* temp1 = cntxt->buffer1 + 4 * NMGL_SIZE;
	float* temp2 = cntxt->buffer2 + 4 * NMGL_SIZE;
	float* temp3 = cntxt->buffer3 + 6 * NMGL_SIZE;

	nmppsSubC_32f(points->x, temp0, lowerLeft.v0, count);
	nmppsSubC_32f(points->y, temp1, lowerLeft.v1, count);
	nmppsConvert_32f32s_rounding(temp0, data->x0 + data->count, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y0 + data->count, 0, count);

	nmblas_scopy (count, (float*)points->z, 1, (float*)data->z + data->count, 1);
	nmblas_scopy(4 * count, (float*)points->colors, 1, (float*)data->color + 4 * data->count, 1);
	
	data->count += count;
	
}