#include "demo3d_nm0.h"
#include "nmpp.h"


#include "stdio.h"
#include "nmblas.h"
#include "assert.h"



#define sort() 	nmppsMerge_32f(lines->x0, lines->y0, temp0, count);	 \
	nmppsMerge_32f(lines->x1, lines->y1, temp1, count);				 \
	sortByY2(temp0, temp1, count);									 \
	split_v2nm32f((v2nm32f*)temp0, 1, lines->x0, lines->y0, count);	 \
	split_v2nm32f((v2nm32f*)temp1, 1, lines->x1, lines->y1, count);	 \

SECTION(".text_demo3d")
void updatePolygonsL(DataForNmpu1* data, Lines* lines, int count, int segX, int segY){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	float* temp0 = cntxt->buffer0 + 2 * NMGL_SIZE;
	float* temp1 = cntxt->buffer1 + 2 * NMGL_SIZE;
	float* temp2 = cntxt->buffer2 + 2 * NMGL_SIZE;
	float* temp3 = cntxt->buffer3 + 6 * NMGL_SIZE;

	sort();
	nmppsSubC_32f(lines->x0, temp0, cntxt->windowInfo.x0_f[segX], count);
	nmppsSubC_32f(lines->y0, temp1, cntxt->windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_rounding(temp0, data->x0, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y0, 0, count);
	nmppsSubC_32f(lines->x1, temp0, cntxt->windowInfo.x0_f[segX], count);
	nmppsSubC_32f(lines->y1, temp1, cntxt->windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_rounding(temp0, data->x1, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y1, 0, count);

	nmblas_scopy (count, (float*)lines->z, 1, (float*)data->z, 1);
	nmblas_scopy(4 * count, (float*)lines->colors, 1, (float*)data->color, 1);
#ifdef DEBUG
	static unsigned int counter = 0;
	for (int i = 0; i < count; i++) {
		if (ABS(data->x1[i] - data->x0[i]) >= 32 ||
			data->y1[i] - data->y0[i] >= 32) {
			/*printf("updatePolygonsL error. counter=%i, i=%d\n", counter, i);
			printf("x0=%d, y0=%d\n", data->x0[i], data->y0[i]);
			printf("x1=%d, y1=%d\n", data->x1[i], data->y1[i]);
			printf("count=%d\n", count);*/
			/*DataForNmpu1Element elem;
			elem.x0 = data->x0[i];
			elem.x1 = data->x1[i];
			elem.y0 = data->y0[i];
			elem.y1 = data->y1[i];
			elem.z = data->z[i];
			elem.color[0] = data->color[4 * i + 0];
			elem.color[1] = data->color[4 * i + 1];
			elem.color[2] = data->color[4 * i + 2];
			elem.color[3] = data->color[4 * i + 3];
			elem.counter = counter;
			elem.i = i;
			throw elem;*/

			data->x0[i] = 0;
			data->x1[i] = 0;
			data->y0[i] = 0;
			data->y1[i] = 0;
		}
	}
	counter++;
#endif // DEBUG

	
	
	data->count = count;
	
}