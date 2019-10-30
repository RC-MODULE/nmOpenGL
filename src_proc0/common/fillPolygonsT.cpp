#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmgl_data0.h"

#include "stdio.h"
#include "nmblas.h"

inline void getNumbersPtrns(float* x0, float* y0, float* x1, float* y1, float* minX, int* dydx, float* dst, int count) {
	float* temp0 = cntxt.buffer0 + 3 * NMGL_SIZE;
	float* temp1 = cntxt.buffer1 + 3 * NMGL_SIZE;
	float* temp2 = cntxt.buffer2 + 3 * NMGL_SIZE;
	int* temp0_int = (int*)temp0;

	nmppsSub_32f(x1, x0, temp0, count);		//dx
	nmppsSub_32f(y1, y0, temp1, count);		//dy
	nmppsMulC_AddV_AddC_32f((nm32f*)temp1, 2 * WIDTH_PTRN, (nm32f*)temp0, WIDTH_PTRN, (nm32f*)temp2, count);		//i
	nmppsConvert_32f32s_rounding(temp2, temp0_int, 0, count);
	remap_32u((nm32u*)dydx, (nm32u*)temp1, temp0_int, count);
	nmppsConvert_32s32f((nm32s*)temp1, temp0, count);
	nmppsSub_32f(x0, minX, temp2, count);					//x0
	nmppsAdd_32f(temp0, temp2, dst, count);				//dydx(dx, dy) + x0
}

inline void sort(Triangles* triangles, int count) {
	float* pAxy = cntxt.buffer0 + 4 * NMGL_SIZE;
	float* pBxy = cntxt.buffer1 + 4 * NMGL_SIZE;
	float* pCxy = cntxt.buffer2 + 4 * NMGL_SIZE;
	nmppsMerge_32f(triangles->x0, triangles->y0, pCxy, count);
	nmppsMerge_32f(triangles->x1, triangles->y1, pBxy, count);
	nmppsMerge_32f(triangles->x2, triangles->y2, pAxy, count);
	sortByY3(pCxy, pBxy, pAxy, count);
	split_v2nm32f((v2nm32f*)pCxy, 1, triangles->x0, triangles->y0, count);
	split_v2nm32f((v2nm32f*)pBxy, 1, triangles->x1, triangles->y1, count);
	split_v2nm32f((v2nm32f*)pAxy, 1, triangles->x2, triangles->y2, count);
}

inline void getCrossProducts() {

}

SECTION(".text_demo3d")
void fillPolygonsT(Polygons* poly, Triangles* triangles, int count, int segX, int segY){
	sort(triangles, count);

	float* temp0 = cntxt.buffer0 + 7 * NMGL_SIZE;
	float* temp1 = cntxt.buffer1 + 7 * NMGL_SIZE;
	float* temp2 = cntxt.buffer2 + 7 * NMGL_SIZE;

	float* dyBA_float = cntxt.buffer0 + 5 * NMGL_SIZE;
	float* dyCA_float = cntxt.buffer2 + 5 * NMGL_SIZE;
	float* dyCB_float = cntxt.buffer1 + 6 * NMGL_SIZE;
	nmppsSub_32f(triangles->y2, triangles->y1, dyBA_float, count);
	nmppsSub_32f(triangles->y2, triangles->y0, dyCA_float, count);
	nmppsSub_32f(triangles->y1, triangles->y0, dyCB_float, count);

	nmppsMulC_32f(dyBA_float, temp0, WIDTH_PTRN / 16, count);
	nmppsMulC_32f(dyCA_float, temp1, WIDTH_PTRN / 16, count);
	nmppsMulC_32f(dyCB_float, temp2, WIDTH_PTRN / 16, count);
	nmppsConvert_32f32s_rounding(temp0, poly->ptrnSizesOf32_12, 0, count);
	nmppsConvert_32f32s_rounding(temp1, poly->ptrnSizesOf32_02, 0, count);
	nmppsConvert_32f32s_rounding(temp2, poly->ptrnSizesOf32_01, 0, count);

	float* crossProducts = cntxt.buffer3 + 5 * NMGL_SIZE;
	nmppsSub_32f(triangles->x2, triangles->x0, temp0, count);
	nmppsSub_32f(triangles->x1, triangles->x0, temp1, count);
	nmppsMul_Mul_Sub_32f(temp0, dyCB_float, dyCA_float, temp1, (float*)crossProducts, count);
	
	float* minX = cntxt.buffer0 + 4 * NMGL_SIZE;
	float* maxX = cntxt.buffer1 + 4 * NMGL_SIZE; 
	findMinMax3(triangles->x0, triangles->x1, triangles->x2, minX, maxX, count);

	float* minY = cntxt.buffer3 + 7 * NMGL_SIZE;
	int* dydx = (int*)cntxt.buffer3 + 8 * NMGL_SIZE;
	nmblas_scopy(2 * WIDTH_PTRN * (HEIGHT_PTRN + 2), (float*)cntxt.patterns->table_dydx, 1, (float*)dydx, 1);
	getNumbersPtrns(triangles->x0, triangles->y0, triangles->x1, triangles->y1, minX, dydx, temp0, count);
	getNumbersPtrns(triangles->x0, triangles->y0, triangles->x2, triangles->y2, minX, dydx, temp1, count);
	getNumbersPtrns(triangles->x1, triangles->y1, triangles->x2, triangles->y2, minX, dydx, temp2, count);
	ternaryLt0_AddC_AddC_32f(crossProducts, temp0, NPATTERNS / 2, 0, minY, count);
	nmppsConvert_32f32s_rounding(minY, poly->numbersPattrns01, 0, count);
	ternaryLt0_AddC_AddC_32f(crossProducts, temp2, NPATTERNS / 2, 0, temp0, count);
	nmppsConvert_32f32s_rounding(temp0, poly->numbersPattrns12, 0, count);
	ternaryLt0_AddC_AddC_32f(crossProducts, temp1, 0, NPATTERNS / 2, temp2, count);
	nmppsConvert_32f32s_rounding(temp2, poly->numbersPattrns02, 0, count);


	nmppsSubC_32f(triangles->y0, temp0, cntxt.windowInfo.y0_f[segY], count);
	absIfNegElse0_32f(temp0, temp1, count);
	nmppsConvert_32f32s_rounding(temp1, poly->offsetsY, 0, count);

	nmppsSubC_32f(minX, temp0, cntxt.windowInfo.x0_f[segX], count);
	absIfNegElse0_32f(temp0, temp1, count);
	nmppsConvert_32f32s_rounding(temp1, poly->offsetsX, 0, count);


	clamp_32f(minX, cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.x1_f[segX], minX, count);
	clamp_32f(maxX, cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.x1_f[segX], temp1, count);
	nmppsSub_32f(temp1, minX, temp2, count);
	nmppsConvert_32f32s_rounding(temp2, poly->widths, 0, count);

	clamp_32f(triangles->y0, cntxt.windowInfo.y0_f[segY], cntxt.windowInfo.y1_f[segY], minY, count);
	clamp_32f(triangles->y2, cntxt.windowInfo.y0_f[segY], cntxt.windowInfo.y1_f[segY], temp1, count);
	nmppsSub_32f(temp1, minY, temp2, count);
	nmppsConvert_32f32s_rounding(temp2, poly->heights, 0, count);

	nmppsSubC_32f(minX, temp0, cntxt.windowInfo.x0_f[segX], count);
	nmppsSubC_32f(minY, temp1, cntxt.windowInfo.y0_f[segY], count);
	int segWidth = cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX];
	nmppsMulC_AddV_32f(temp1, temp0, temp2, segWidth, count);
	nmppsConvert_32f32s_rounding(temp2, poly->pointInImage, 0, count);


	poly->count = count;
}