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
	int* temp1_int = (int*)temp1;

	doubleSub_32f(x1, y1, x0, y0, temp0, temp1, count);
	nmppsMulC_AddV_AddC_32f((nm32f*)temp1, 2 * WIDTH_PTRN, (nm32f*)temp0, WIDTH_PTRN, (nm32f*)temp2, count);		//i
	nmppsConvert_32f32s_rounding(temp2, temp0_int, 0, count);
	remap_32u((nm32u*)dydx, (nm32u*)temp1_int, temp0_int, count);
	nmppsConvert_32s32f(temp1_int, temp0, count);
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

SECTION(".text_demo3d")
void fillPolygonsT(Polygons* poly, Triangles* triangles, int count, int segX, int segY){
	sort(triangles, count);

	float* temp0 = cntxt.buffer0 + 7 * NMGL_SIZE;
	float* temp1 = cntxt.buffer1 + 7 * NMGL_SIZE;
	float* temp2 = cntxt.buffer2 + 7 * NMGL_SIZE;
	float* temp3 = cntxt.buffer3 + 9 * NMGL_SIZE;

	float* dyBA_float = cntxt.buffer0 + 5 * NMGL_SIZE;
	float* dyCA_float = cntxt.buffer2 + 5 * NMGL_SIZE;
	float* dyCB_float = cntxt.buffer1 + 6 * NMGL_SIZE;
	doubleSub_32f(triangles->y2, triangles->y2, triangles->y1, triangles->y0, dyBA_float, dyCA_float, count);
	nmppsSub_32f(triangles->y1, triangles->y0, dyCB_float, count);

	tripleMulC_32f(dyBA_float, dyCA_float, dyCB_float, WIDTH_PTRN / 16, temp0, temp1, temp2, count);
	nmppsConvert_32f32s_rounding(temp0, poly->ptrnSizesOf32_12, 0, count);
	nmppsConvert_32f32s_rounding(temp1, poly->ptrnSizesOf32_02, 0, count);
	nmppsConvert_32f32s_rounding(temp2, poly->ptrnSizesOf32_01, 0, count);

	float* crossProducts = cntxt.buffer3 + 5 * NMGL_SIZE;
	doubleSub_32f(triangles->x2, triangles->x1, triangles->x0, triangles->x0, temp0, temp1, count);
	nmppsMul_Mul_Sub_32f(temp0, dyCB_float, dyCA_float, temp1, (float*)crossProducts, count);
	
	float* minX = cntxt.buffer0 + 4 * NMGL_SIZE;
	float* maxX = cntxt.buffer1 + 4 * NMGL_SIZE; 
	findMinMax3(triangles->x0, triangles->x1, triangles->x2, minX, maxX, count);

	int* dydx = (int*)cntxt.buffer3 + 8 * NMGL_SIZE;
	nmblas_scopy(2 * WIDTH_PTRN * (HEIGHT_PTRN + 2), (float*)cntxt.patterns->table_dydx, 1, (float*)dydx, 1);
	getNumbersPtrns(triangles->x0, triangles->y0, triangles->x1, triangles->y1, minX, dydx, temp0, count);
	getNumbersPtrns(triangles->x0, triangles->y0, triangles->x2, triangles->y2, minX, dydx, temp1, count);
	getNumbersPtrns(triangles->x1, triangles->y1, triangles->x2, triangles->y2, minX, dydx, temp2, count);

	ternaryLt0_AddC_AddC_32f(crossProducts, temp0, NPATTERNS / 2, 0, temp3, count);
	nmppsConvert_32f32s_rounding(temp3, poly->numbersPattrns01, 0, count);
	ternaryLt0_AddC_AddC_32f(crossProducts, temp2, NPATTERNS / 2, 0, temp0, count);
	nmppsConvert_32f32s_rounding(temp0, poly->numbersPattrns12, 0, count);
	ternaryLt0_AddC_AddC_32f(crossProducts, temp1, 0, NPATTERNS / 2, temp2, count);
	nmppsConvert_32f32s_rounding(temp2, poly->numbersPattrns02, 0, count);

	doubleSubC_32f(triangles->y0, minX, cntxt.windowInfo.y0_f[segY], cntxt.windowInfo.x0_f[segX], temp0, temp1, count);
	doubleAbsIfNegElse0_32f(temp0, temp1, temp2, temp3, count);
	nmppsConvert_32f32s_rounding(temp2, poly->offsetsY, 0, count);
	nmppsConvert_32f32s_rounding(temp3, poly->offsetsX, 0, count);

	float* minY = cntxt.buffer3 + 7 * NMGL_SIZE;
	doubleClamp_32f(minX, maxX,
		cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.x1_f[segX],
		minX, temp0, count);	
	doubleClamp_32f(triangles->y0, triangles->y2,
		cntxt.windowInfo.y0_f[segY], cntxt.windowInfo.y1_f[segY],
		minY, temp1, count);
	doubleSub_32f(temp0, temp1, minX, minY, temp2, temp3, count);
	nmppsConvert_32f32s_rounding(temp2, poly->widths, 0, count);
	nmppsConvert_32f32s_rounding(temp3, poly->heights, 0, count);

	doubleSubC_32f(minX, minY, cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.y0_f[segY], temp0, temp1, count);
	int segWidth = cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX];
	nmppsMulC_AddV_32f(temp1, temp0, temp2, segWidth, count);
	nmppsConvert_32f32s_rounding(temp2, poly->pointInImage, 0, count);


	poly->count = count;
}