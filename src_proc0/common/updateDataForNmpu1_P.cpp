#include "demo3d_nm0.h"
#include "nmpp.h"


#include "stdio.h"
#include "nmblas.h"

void updatePolygonsP(PolygonsOld* data, Points* points, int count, int segX, int segY) {

}

SECTION(".text_demo3d")
//void updateDataForNmpu1_T(DataForNmpu1* data, Triangles* triangles, int count, int segX, int segY){
void updatePolygonsP(DataForNmpu1* data, Points* points, int count, int segX, int segY){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	float* temp0 = cntxt->buffer0 + 2 * NMGL_SIZE;
	float* temp1 = cntxt->buffer1 + 2 * NMGL_SIZE;
	float* temp2 = cntxt->buffer2 + 2 * NMGL_SIZE;
	float* temp3 = cntxt->buffer3 + 6 * NMGL_SIZE;

	nmppsSubC_32f(points->x0, temp0, cntxt->windowInfo.x0_f[segX], count);
	nmppsSubC_32f(points->y0, temp1, cntxt->windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_rounding(temp0, data->x0, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y0, 0, count);

	nmblas_scopy (count, (float*)points->z, 1, (float*)data->z, 1);
	nmblas_scopy(4 * count, (float*)points->colors, 1, (float*)data->color, 1);
	
	data->count = count;
	
}