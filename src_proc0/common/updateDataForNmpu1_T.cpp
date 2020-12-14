#include "demo3d_nm0.h"
#include "nmpp.h"


#include "stdio.h"
#include "nmblas.h"
#include "nmprofiler.h"
#include "assert.h"
#include "math.h"


#ifndef TEXTURE_ENABLED
#define sort() 	nmppsMerge_32f(triangles->x0, triangles->y0, temp0, count);	 \
	nmppsMerge_32f(triangles->x1, triangles->y1, temp1, count);				 \
	nmppsMerge_32f(triangles->x2, triangles->y2, temp2, count);				 \
	sortByY3(temp0, temp1, temp2, count);									 \
	split_v2nm32f((v2nm32f*)temp0, 1, triangles->x0, triangles->y0, count);	 \
	split_v2nm32f((v2nm32f*)temp1, 1, triangles->x1, triangles->y1, count);	 \
	split_v2nm32f((v2nm32f*)temp2, 1, triangles->x2, triangles->y2, count)

#else //TEXTURE_ENABLED

#define sort() 	nmppsMerge_32f(triangles->x0, triangles->y0, temp0, count);	 \
	nmppsMerge_32f(triangles->x1, triangles->y1, temp1, count);				 \
	nmppsMerge_32f(triangles->x2, triangles->y2, temp2, count);				 \
	if (cntxt->texState.textureEnabled == 0) {                                \
		sortByY3(temp0, temp1, temp2, count);									 \
	}																		 \
	else {																	 \
		sortByY5(temp0, temp1, temp2, \
				 triangles->s0, triangles->t0, \
				 triangles->s1, triangles->t1, \
				 triangles->s2, triangles->t2, \
				 triangles->w0, triangles->w1, triangles->w2, count);						\
	}																		 \
	split_v2nm32f((v2nm32f*)temp0, 1, triangles->x0, triangles->y0, count);	 \
	split_v2nm32f((v2nm32f*)temp1, 1, triangles->x1, triangles->y1, count);	 \
	split_v2nm32f((v2nm32f*)temp2, 1, triangles->x2, triangles->y2, count)
#endif //TEXTURE_ENABLED



SECTION(".text_demo3d")
//void updateDataForNmpu1_T(DataForNmpu1* data, Triangles* triangles, int count, int segX, int segY){
void updatePolygonsT(DataForNmpu1* data, Triangles* triangles, int count, int segX, int segY){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	float* temp0 = cntxt->buffer0 + 2 * NMGL_SIZE;
	float* temp1 = cntxt->buffer1 + 2 * NMGL_SIZE;
	float* temp2 = cntxt->buffer2 + 2 * NMGL_SIZE;
	float* temp3 = cntxt->buffer3 + 6 * NMGL_SIZE;

#ifdef TEXTURE_ENABLED
//	if (cntxt->texState.textureEnabled) {
//		for (int i = 0; i < count; i++) {
//			printf("before sort ");
//			printf("w0 %f ", triangles->w0[i]);
//			printf("w1 %f ", triangles->w1[i]);
//			printf("w2 %f ", triangles->w2[i]);
//			printf("s0 %f ", triangles->s0[i]);
//			printf("t0 %f ", triangles->t0[i]);
//			printf("s1 %f ", triangles->s1[i]);
//			printf("t1 %f ", triangles->t1[i]);
//			printf("s2 %f ", triangles->s2[i]);
//			printf("t2 %f ", triangles->t2[i]);
//			printf("\n");
//		}
//	}
#endif //TEXTURE_ENABLED

	sort();


#ifdef TEXTURE_ENABLED
//	if (cntxt->texState.textureEnabled) {
//		for (int i = 0; i < count; i++) {
//			printf("after sort ");
//			printf("w0 %f ", triangles->w0[i]);
//			printf("w1 %f ", triangles->w1[i]);
//			printf("w2 %f ", triangles->w2[i]);
//			printf("s0 %f ", triangles->s0[i]);
//			printf("t0 %f ", triangles->t0[i]);
//			printf("s1 %f ", triangles->s1[i]);
//			printf("t1 %f ", triangles->t1[i]);
//			printf("s2 %f ", triangles->s2[i]);
//			printf("t2 %f ", triangles->t2[i]);
//			printf("\n");
//		}
//	}
#endif //TEXTURE_ENABLED

	nmppsSub_32f(triangles->x1, triangles->x0, temp0, count);
	nmppsSub_32f(triangles->x2, triangles->x0, temp1, count);
	nmppsSub_32f(triangles->y1, triangles->y0, temp2, count);
	nmppsSub_32f(triangles->y2, triangles->y0, temp3, count);
	nmppsMul_Mul_Sub_32f(temp1, temp2, temp3, temp0, (float*)data->crossProducts + data->count, count);

	nmppsSubC_32f(triangles->x0, temp0, cntxt->windowInfo.x0_f[segX], count);
	nmppsSubC_32f(triangles->y0, temp1, cntxt->windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_rounding(temp0, data->x0 + data->count, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y0 + data->count, 0, count);
	nmppsSubC_32f(triangles->x1, temp0, cntxt->windowInfo.x0_f[segX], count);
	nmppsSubC_32f(triangles->y1, temp1, cntxt->windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_rounding(temp0, data->x1 + data->count, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y1 + data->count, 0, count);
	nmppsSubC_32f(triangles->x2, temp0, cntxt->windowInfo.x0_f[segX], count);
	nmppsSubC_32f(triangles->y2, temp1, cntxt->windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_rounding(temp0, data->x2 + data->count, 0, count);
	nmppsConvert_32f32s_rounding(temp1, data->y2 + data->count, 0, count);
	
	nmblas_scopy (count, (float*)triangles->z, 1, (float*)data->z + data->count, 1);
	nmblas_scopy(4 * count, (float*)triangles->colors, 1, (float*)data->color + 4 * data->count, 1);

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
