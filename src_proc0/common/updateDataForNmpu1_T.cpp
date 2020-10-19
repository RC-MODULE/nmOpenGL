#include "demo3d_nm0.h"
#include "nmpp.h"


#include "stdio.h"
#include "nmblas.h"
#include "nmprofiler.h"
#include "assert.h"
#include "math.h"



#define sort() 	nmppsMerge_32f(triangles->x0, triangles->y0, temp0, count);	 \
	nmppsMerge_32f(triangles->x1, triangles->y1, temp1, count);				 \
	nmppsMerge_32f(triangles->x2, triangles->y2, temp2, count);				 \
	sortByY3(temp0, temp1, temp2, count);									 \
	split_v2nm32f((v2nm32f*)temp0, 1, triangles->x0, triangles->y0, count);	 \
	split_v2nm32f((v2nm32f*)temp1, 1, triangles->x1, triangles->y1, count);	 \
	split_v2nm32f((v2nm32f*)temp2, 1, triangles->x2, triangles->y2, count)

void modified(float* x, float* y, int size) {
	for (int i = 0; i < size; i++) {
		
	}
}

void modifiedSide(float* x0, float* y0, float* x1, float* y1, int size, int add) {
	for (int i = 0; i < size; i++) {
		float deltaX = x1[i] - x0[i];
		float deltaY = y1[i] - y0[i];
		float signX = (deltaX < 0) ? -1 : 1;
		float signY = (deltaY < 0) ? -1 : 1;
		float k = deltaY / deltaX;
		float kInv = deltaX / deltaY;
		float b = deltaY - k * deltaX;
		float check0 = ABS(y0[i] - floor(y0[i])) + ABS(x0[i] - floor(x0[i]));
		float check1 = ABS(y1[i] - floor(y1[i])) + ABS(x1[i] - floor(x1[i]));
		if (check0 <= 0.5 || check1 <= 0.5) {
			b -= k;
			x0[i] += 1;
			y0[i] = k * x0[i] + b;
			x1[i] += 1;
			y1[i] = k * x0[i] + b;
			/*if (ABS(deltaX) > ABS(deltaY)) {
				if (add) {

					x0[i] += signX;
					y0[i] += k * signX;
					//x1[i] += signX;
					//y1[i] += k * signX;
				}
				else {
					x0[i] -= signX;
					y0[i] -= k * signX;
					//x1[i] -= signX;
					//y1[i] -= k * signX;
				}
				
			}
			if (ABS(deltaX) < ABS(deltaY)) {
				if (add) {
					y0[i] += signY;
					x0[i] += signY  * kInv;
					//y1[i] += signY;
					//x1[i] += signY  * kInv;
				}
				else {
					y0[i] -= signY;
					x0[i] -= signY  * kInv;
					//y1[i] -= signY;
					//x1[i] -= signY  * kInv;
				}
			}*/
		}
	}
}

SECTION(".text_demo3d")
//void updateDataForNmpu1_T(DataForNmpu1* data, Triangles* triangles, int count, int segX, int segY){
void updatePolygonsT(DataForNmpu1* data, Triangles* triangles, int count, int segX, int segY){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	float* temp0 = cntxt->buffer0 + 2 * NMGL_SIZE;
	float* temp1 = cntxt->buffer1 + 2 * NMGL_SIZE;
	float* temp2 = cntxt->buffer2 + 2 * NMGL_SIZE;
	float* temp3 = cntxt->buffer3 + 6 * NMGL_SIZE;

	sort();

	nmppsSub_32f(triangles->x1, triangles->x0, temp0, count);
	nmppsSub_32f(triangles->x2, triangles->x0, temp1, count);
	nmppsSub_32f(triangles->y1, triangles->y0, temp2, count);
	nmppsSub_32f(triangles->y2, triangles->y0, temp3, count);
	nmppsMul_Mul_Sub_32f(temp1, temp2, temp3, temp0, (float*)data->crossProducts, count);

	float* x0Local = cntxt->buffer0 + 3 * NMGL_SIZE;
	float* y0Local = cntxt->buffer0 + 4 * NMGL_SIZE;
	float* dx01 = cntxt->buffer1 + 3 * NMGL_SIZE;
	float* dy01 = cntxt->buffer1 + 4 * NMGL_SIZE;
	float* dx12 = cntxt->buffer1 + 5 * NMGL_SIZE;
	float* dy12 = cntxt->buffer1 + 6 * NMGL_SIZE;
	float* x0LocalFloor = cntxt->buffer0 + 5 * NMGL_SIZE;
	float* y0LocalFloor = cntxt->buffer0 + 6 * NMGL_SIZE;
	int* cnvTemp0 = (int*)cntxt->buffer3 + 7 * NMGL_SIZE;
	int* cnvTemp1 = (int*)cntxt->buffer3 + 8 * NMGL_SIZE;

	
	nmppsSubC_32f(triangles->x0, x0Local, cntxt->windowInfo.x0_f[segX], count);				
	nmppsSubC_32f(triangles->y0, y0Local, cntxt->windowInfo.y0_f[segY], count);				
	nmppsConvert_32f32s_floor(triangles->x0, cnvTemp0, 0, count);
	nmppsConvert_32f32s_floor(triangles->y0, cnvTemp1, 0, count);
	nmppsConvert_32s32f(cnvTemp0, triangles->x0, count);
	nmppsConvert_32s32f(cnvTemp1, triangles->y0, count);
	nmppsConvert_32f32s_floor(x0Local, data->x0, 0, count);	
	nmppsConvert_32f32s_floor(y0Local, data->y0, 0, count);	
	nmppsConvert_32f32s_floor(x0Local, cnvTemp0, 0, count);
	nmppsConvert_32f32s_floor(y0Local, cnvTemp1, 0, count);
	nmppsConvert_32s32f(cnvTemp0, x0LocalFloor, count);
	nmppsConvert_32s32f(cnvTemp1, y0LocalFloor, count);
	

	nmppsSub_32f(triangles->x1, triangles->x0, temp0, count);
	nmppsAdd_32f(temp0, x0LocalFloor, temp1, count);
	nmppsConvert_32f32s_floor(temp1, data->x1, 0, count);
	nmppsSub_32f(triangles->y1, triangles->y0, temp0, count);
	nmppsAdd_32f(temp0, y0LocalFloor, temp1, count);
	nmppsConvert_32f32s_floor(temp1, data->y1, 0, count);

	nmppsSub_32f(triangles->x2, triangles->x0, temp0, count);
	nmppsAdd_32f(temp0, x0LocalFloor, temp1, count);
	nmppsConvert_32f32s_floor(temp1, data->x2, 0, count);
	nmppsSub_32f(triangles->y2, triangles->y0, temp0, count);
	nmppsAdd_32f(temp0, y0LocalFloor, temp1, count);
	nmppsConvert_32f32s_floor(temp1, data->y2, 0, count);
	

	/*nmblas_scopy(count, cntxt->buffer1, 1, (float*)data->x0, 1);							
	nmppsSub_32f(cntxt->buffer0, triangles->x0, cntxt->buffer2, count);						//errorX = (x0 - x0Seg) - floor(x0 - x0Seg)
	nmppsSub_32f(triangles->x1, triangles->x0, cntxt->buffer0, count);						// dx01
	nmppsAdd_32f(cntxt->buffer0, cntxt->buffer2, cntxt-> buffer1, count);					// dx01 + errorX
	nmppsAdd_32f(cntxt->buffer0, cntxt->buffer2, cntxt->buffer1, count);					// (dx01 + errorX) + (x0 - x0Seg)


	nmppsConvert_32f32s_floor(triangles->x0, cntxt->buffer0, 0, count);
	nmppsConvert_32f32s_floor(triangles->y0, cntxt->buffer1, 0, count);
	nmppsConvert_32s32f(cntxt->buffer0, cntxt->buffer2, count);
	nmppsConvert_32s32f(cntxt->buffer0, cntxt->buffer2, count);*/

	/*for (int i = 0; i < count; i++) {
		//cntxt->buffer0[i] = errorX
		//cntxt->buffer1[i] = errorY
		int x0 = (int)floor(triangles->x0[i] - cntxt->windowInfo.x0_f[segX]);
		int y0 = (int)floor(triangles->y0[i] - cntxt->windowInfo.y0_f[segY]);
		data->x0[i] = x0;
		data->y0[i] = y0;
		int dx01 = (int)floor(triangles->x1[i] - floor(triangles->x0[i]));
		int dy01 = (int)floor(triangles->y1[i] - floor(triangles->x0[i]));
		data->x1[i] = x0 + dx01;
		data->y1[i] = y0 + dy01;
		int dx12 = (int)floor(triangles->x2[i] - floor(triangles->x0[i]));
		int dy12 = (int)floor(triangles->y2[i] - floor(triangles->x0[i]));
		data->x2[i] = x0 + dx12;
		data->y2[i] = y0 + dy12;
	}*/

	
	/*nmppsSubC_32f(triangles->x0, temp0, cntxt->windowInfo.x0_f[segX], count);
	nmppsSubC_32f(triangles->y0, temp1, cntxt->windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_floor(temp0, data->x0, 0, count);
	nmppsConvert_32f32s_floor(temp1, data->y0, 0, count);
	nmppsSubC_32f(triangles->x1, temp0, cntxt->windowInfo.x0_f[segX], count);
	nmppsSubC_32f(triangles->y1, temp1, cntxt->windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_rounding(temp0, data->x1, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y1, 0, count);
	nmppsSubC_32f(triangles->x2, temp0, cntxt->windowInfo.x0_f[segX], count);
	nmppsSubC_32f(triangles->y2, temp1, cntxt->windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_floor(temp0, data->x2, 0, count);
	nmppsConvert_32f32s_floor(temp1, data->y2, 0, count);*/

	

	
	nmblas_scopy (count, (float*)triangles->z, 1, (float*)data->z, 1);
	nmblas_scopy(4 * count, (float*)triangles->colors, 1, (float*)data->color, 1);

#ifdef DEBUG
	static unsigned int counter = 0;
	for (int i = 0; i < count; i++) {
		if(	ABS((data->x1[i] - data->x0[i])) > 32 ||
			ABS((data->x2[i] - data->x0[i])) > 32 ||
			ABS((data->x2[i] - data->x1[i])) > 32 ||
			(data->y1[i] - data->y0[i]) > 32 ||
			(data->y2[i] - data->y0[i]) > 32 ||
			(data->y2[i] - data->y1[i]) > 32){
				//printf("updatePolygonsT error. counter=%i, i=%d\n", counter, i);
				data->x0[i] = 0;
				data->x1[i] = 0;
				data->x2[i] = 0;
				data->y0[i] = 0;
				data->y1[i] = 0;
				data->y2[i] = 0;
			}
	}
	counter++;
#endif // DEBUG

	
	data->count = count;

}