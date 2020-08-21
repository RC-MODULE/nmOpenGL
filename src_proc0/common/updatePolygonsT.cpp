#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmgl_data0.h"

#include "stdio.h"
#include "nmblas.h"



#define sort() 	nmppsMerge_32f(triangles->x0, triangles->y0, temp0, count);	 \
	nmppsMerge_32f(triangles->x1, triangles->y1, temp1, count);				 \
	nmppsMerge_32f(triangles->x2, triangles->y2, temp2, count);				 \
	sortByY3(temp0, temp1, temp2, count);									 \
	split_v2nm32f((v2nm32f*)temp0, 1, triangles->x0, triangles->y0, count);	 \
	split_v2nm32f((v2nm32f*)temp1, 1, triangles->x1, triangles->y1, count);	 \
	split_v2nm32f((v2nm32f*)temp2, 1, triangles->x2, triangles->y2, count)

SECTION(".text_demo3d")
void updatePolygonsT(Polygons* poly, Triangles* triangles, int count, int segX, int segY){
#ifdef TEXTURE_ENABLED
	if (cntxt.texState.textureEnabled){
		nmblas_scopy(count, (float*)triangles->x0, 1, (float*)poly->x0 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->y0, 1, (float*)poly->y0 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->x1, 1, (float*)poly->x1 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->y1, 1, (float*)poly->y1 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->x2, 1, (float*)poly->x2 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->y2, 1, (float*)poly->y2 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->zEye0, 1, (float*)poly->zEye0 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->zEye1, 1, (float*)poly->zEye1 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->zEye2, 1, (float*)poly->zEye2 + poly->count, 1);

		nmblas_scopy(count, (float*)triangles->s0, 1, (float*)poly->s0 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->t0, 1, (float*)poly->t0 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->s1, 1, (float*)poly->s1 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->t1, 1, (float*)poly->t1 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->s2, 1, (float*)poly->s2 + poly->count, 1);
		nmblas_scopy(count, (float*)triangles->t2, 1, (float*)poly->t2 + poly->count, 1);
	}
#endif //TEXTURE_ENABLED

	float* temp0 = cntxt.buffer0 + 2 * NMGL_SIZE;
	float* temp1 = cntxt.buffer1 + 2 * NMGL_SIZE;
	float* temp2 = cntxt.buffer2 + 2 * NMGL_SIZE;
	float* temp3 = cntxt.buffer3 + 6 * NMGL_SIZE;
	float* dy12 = cntxt.buffer1 + 4 * NMGL_SIZE;
	float* dy02 = cntxt.buffer2 + 4 * NMGL_SIZE;
	float* dy01 = cntxt.buffer0 + 4 * NMGL_SIZE;
	float* dx12 = cntxt.buffer2 + 5 * NMGL_SIZE;
	float* dx02 = cntxt.buffer0 + 5 * NMGL_SIZE;
	float* dx01 = cntxt.buffer1 + 5 * NMGL_SIZE;
	float* crossProducts = cntxt.buffer3 + 7 * NMGL_SIZE;
	float* minX = cntxt.buffer0 + 6 * NMGL_SIZE;
	float* maxX = cntxt.buffer1 + 6 * NMGL_SIZE;
	float* minY = cntxt.buffer3 + 8 * NMGL_SIZE;	
	int* dydx = (int*)cntxt.buffer3 + 9 * NMGL_SIZE;
	
    //dydx should has enough size to hold 2 * WIDTH_PTRN * (HEIGHT_PTRN + 2) = 2176 floats
	nmblas_scopy(2 * WIDTH_PTRN * (HEIGHT_PTRN + 2), (float*)cntxt.patterns->table_dydx, 1, (float*)dydx, 1);
	int segWidth = cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX];

	sort();
	doubleSub_32f(triangles->y2, triangles->y2, triangles->y1, triangles->y0, dy12, dy02, count);
	nmppsSub_32f(triangles->y1, triangles->y0, dy01, count);

	tripleMulC_32f(dy12, dy02, dy01, WIDTH_PTRN / 16, temp0, temp1, temp2, count);
	//nmppsConvert_32f32s_rounding(temp0, poly->ptrnSizesOf32_12 + poly->count, 0, count);
	nmppsConvert_32f32s_rounding(temp1, poly->ptrnSizesOf32_02 + poly->count, 0, count);
	nmppsConvert_32f32s_rounding(temp2, poly->ptrnSizesOf32_01 + poly->count, 0, count);
	
	doubleSub_32f(triangles->x2, triangles->x1, triangles->x0, triangles->x0, dx02, dx01, count);
	nmppsSub_32f(triangles->x2, triangles->x1, dx12, count);
	nmppsMul_Mul_Sub_32f(dx02, dy01, dy02, dx01, (float*)crossProducts, count);

	findMinMax3(triangles->x0, triangles->x1, triangles->x2, minX, maxX, count);

	nmppsMulC_AddV_AddC_32f(dy01, 2 * WIDTH_PTRN, dx01, WIDTH_PTRN, temp0, count);
	nmppsMulC_AddV_AddC_32f(dy12, 2 * WIDTH_PTRN, dx12, WIDTH_PTRN, temp1, count);
	nmppsMulC_AddV_AddC_32f(dy02, 2 * WIDTH_PTRN, dx02, WIDTH_PTRN, temp2, count);
	nmppsConvert_32f32s_rounding(temp0, (nm32s*)temp3, 0, count);
	nmppsConvert_32f32s_rounding(temp1, (nm32s*)temp0, 0, count);
	nmppsConvert_32f32s_rounding(temp2, (nm32s*)temp1, 0, count);
	remap_32u((nm32u*)dydx, (nm32u*)temp2, (nm32s*)temp3, count);
	remap_32u((nm32u*)dydx, (nm32u*)temp3, (nm32s*)temp0, count);
	remap_32u((nm32u*)dydx, (nm32u*)temp0, (nm32s*)temp1, count);
	nmppsConvert_32s32f((nm32s*)temp2, temp1, count);
	nmppsConvert_32s32f((nm32s*)temp3, temp2, count);
	nmppsConvert_32s32f((nm32s*)temp0, temp3, count);
	doubleSub_32f(triangles->x0, triangles->x1, 
		minX, minX, 
		temp2 + NMGL_SIZE, temp0 + NMGL_SIZE, 
		count);

	nmppsAdd_32f(temp1, temp2 + NMGL_SIZE, temp0, count);
	nmppsAdd_32f(temp2, temp0 + NMGL_SIZE, temp1, count);
	nmppsAdd_32f(temp3, temp2 + NMGL_SIZE, temp1 + NMGL_SIZE, count);
	ternaryLt0_AddC_AddC_32f(crossProducts, temp0, NPATTERNS/2, 0, temp3, count);
	nmppsConvert_32f32s_rounding(temp3, poly->numbersPattrns01 + poly->count, 0, count);
	ternaryLt0_AddC_AddC_32f(crossProducts, temp1, NPATTERNS / 2, 0, temp3, count);
	nmppsConvert_32f32s_rounding(temp3, poly->numbersPattrns12 + poly->count, 0, count);
	ternaryLt0_AddC_AddC_32f(crossProducts, temp1 + NMGL_SIZE, 0, NPATTERNS / 2, temp3, count);
	nmppsConvert_32f32s_rounding(temp3, poly->numbersPattrns02 + poly->count, 0, count);

	doubleSubC_32f(triangles->y0, minX, cntxt.windowInfo.y0_f[segY], cntxt.windowInfo.x0_f[segX], temp0, temp1, count);
	doubleAbsIfNegElse0_32f(temp0, temp1, temp2, temp3, count);
	nmppsConvert_32f32s_rounding(temp2, poly->offsetsY + poly->count, 0, count);
	nmppsConvert_32f32s_rounding(temp3, poly->offsetsX + poly->count, 0, count);

	doubleClamp_32f(minX, maxX,
		cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.x1_f[segX],
		minX, temp0, count);	
	doubleClamp_32f(triangles->y0, triangles->y2,
		cntxt.windowInfo.y0_f[segY], cntxt.windowInfo.y1_f[segY],
		minY, temp1, count);
	doubleSub_32f(temp0, temp1, minX, minY, temp2, temp3, count);
	nmppsConvert_32f32s_rounding(temp2, poly->widths + poly->count, 0, count);
	nmppsConvert_32f32s_rounding(temp3, poly->heights + poly->count, 0, count);

	doubleSubC_32f(minX, minY, cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.y0_f[segY], temp0, temp1, count);

	nmppsMulC_AddV_32f(temp1, temp0, temp2, segWidth, count);
	nmppsConvert_32f32s_rounding(temp2, poly->pointInImage + poly->count, 0, count);

	nmblas_scopy(count, (float*)triangles->z, 1, (float*)poly->z + poly->count, 1);
	nmblas_scopy(4 * count, (float*)triangles->colors, 1, (float*)poly->color + 4 * poly->count, 1);

	poly->count += count;
}
