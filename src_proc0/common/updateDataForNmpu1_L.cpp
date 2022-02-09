/*!
 *  \file updateDataForNmpu1_L.cpp
 *  \author Zhilenkov Ivan
 */
#include "demo3d_nm0.h"
#include "nmpp.h"
#include "math.h"


#include "stdio.h"
#include "nmblas.h"
#include "assert.h"
#include "utility_float.h"



#define sort() 	nmppsMerge_32f(lines->x0, lines->y0, temp0, count);	 \
	nmppsMerge_32f(lines->x1, lines->y1, temp1, count);				 \
	sortByY2(temp0, temp1, count);									 \
	split_v2nm32f((v2nm32f*)temp0, 1, lines->x0, lines->y0, count);	 \
	split_v2nm32f((v2nm32f*)temp1, 1, lines->x1, lines->y1, count);	 \

SECTION(".text_demo3d")
void updatePolygonsL(DataForNmpu1* data, Lines* lines, int count, v2nm32f lowerLeft){
	NMGL_Context_NM0* cntxt = NMGL_Context_NM0::getContext();
	float* temp0 = cntxt->buffer0 + 2 * NMGL_SIZE;
	float* temp1 = cntxt->buffer1 + 2 * NMGL_SIZE;
	float* temp2 = cntxt->buffer2 + 2 * NMGL_SIZE;
	float* temp3 = cntxt->buffer3 + 6 * NMGL_SIZE;

	sort();

	nmppsSubC_32f(lines->x0, temp0, lowerLeft.v0, count);
	nmppsSubC_32f(lines->y0, temp1, lowerLeft.v1, count);
	nmppsConvert_32f32s_rounding(temp0, data->x0 + data->count, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y0 + data->count, 0, count);
	nmppsSubC_32f(lines->x1, temp0, lowerLeft.v0, count);
	nmppsSubC_32f(lines->y1, temp1, lowerLeft.v1, count);
	nmppsConvert_32f32s_rounding(temp0, data->x1 + data->count, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y1 + data->count, 0, count);

	nmblas_scopy (count, (float*)lines->z, 1, (float*)data->z + data->count, 1);
	nmblas_scopy(4 * count, (float*)lines->colors, 1, (float*)data->color + 4 * data->count, 1);
//#ifdef DEBUG
	static unsigned int counter = 0;
	for (int i = 0; i < count; i++) {
		if (ABS(data->x1[i + data->count] - data->x0[i + data->count]) >= 32 ||
			data->y1[i + data->count] - data->y0[i + data->count] >= 32) {
			//printf("updatePolygonsL error. counter=%i, i=%d\n", counter, i);
			data->x0[i+ data->count] = 0;
			data->x1[i+ data->count] = 0;
			data->y0[i+ data->count] = 0;
			data->y1[i+ data->count] = 0;
		}
	}
	counter++;
//#endif // DEBUG

	
	
	data->count += count;
	if (data->count > POLYGONS_SIZE) {
		data->count = data->count;
	}
	
}