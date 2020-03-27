#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmgl_data0.h"
#include "nmblas.h"

#define sort() 	nmppsMerge_32f(lines->x0, lines->y0, temp0, count);	 \
	nmppsMerge_32f(lines->x1, lines->y1, temp1, count);				 \
	sortByY2(temp0, temp1, count);									 \
	split_v2nm32f((v2nm32f*)temp0, 1, lines->x0, lines->y0, count);	 \
	split_v2nm32f((v2nm32f*)temp1, 1, lines->x1, lines->y1, count);	 

SECTION(".text_demo3d")
void updatePolygonsL(Polygons* poly, Lines* lines, int count, int segX, int segY){
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
	float* crossProducts = cntxt.buffer3 + 8 * NMGL_SIZE;
	float* minX = cntxt.buffer0 + 6 * NMGL_SIZE;
	float* maxX = cntxt.buffer1 + 6 * NMGL_SIZE;
	float* minY = cntxt.buffer3 + 9 * NMGL_SIZE;
	int* dydx = (int*)cntxt.buffer3 + 10 * NMGL_SIZE;
	nmblas_scopy(2 * WIDTH_PTRN * (HEIGHT_PTRN + 2), (float*)cntxt.patterns->table_dydx, 1, (float*)dydx, 1);
	int segWidth = cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX];

	sort();
	doubleSub_32f(lines->y1, lines->x1, lines->y0, lines->x0, dy01, dx01, count);
	nmppsMulC_32f(dy01, temp0, WIDTH_PTRN / 16, count);
	nmppsConvert_32f32s_rounding(temp0, poly->ptrnSizesOf32_01 + poly->count, 0, count);

	nmppsMulC_AddV_AddC_32f(dy01, 2 * WIDTH_PTRN, dx01, WIDTH_PTRN, temp0, count);
	nmppsConvert_32f32s_rounding(temp0, (nm32s*)temp3, 0, count);
	remap_32u((nm32u*)dydx, (nm32u*)temp2, (nm32s*)temp3, count);
	nmblas_scopy(count, temp3, 1, (float*)poly->numbersPattrns01 + poly->count, 1);	

	findMinMax2(lines->x0, lines->x1, minX, maxX, count);

	doubleSubC_32f(lines->y0, minX, cntxt.windowInfo.y0_f[segY], cntxt.windowInfo.x0_f[segX], temp0, temp1, count);
	doubleAbsIfNegElse0_32f(temp0, temp1, temp2, temp3, count);
	nmppsConvert_32f32s_rounding(temp2, poly->offsetsY + poly->count, 0, count);
	nmppsConvert_32f32s_rounding(temp3, poly->offsetsX + poly->count, 0, count);

	doubleClamp_32f(minX, maxX,
		cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.x1_f[segX],
		minX, temp0, count);
	doubleClamp_32f(lines->y0, lines->y1,
		cntxt.windowInfo.y0_f[segY], cntxt.windowInfo.y1_f[segY],
		minY, temp1, count);
	doubleSub_32f(temp0, temp1, minX, minY, temp2, temp3, count);
	nmppsConvert_32f32s_rounding(temp2, poly->widths + poly->count, 0, count);
	nmppsConvert_32f32s_rounding(temp3, poly->heights + poly->count, 0, count);

	doubleSubC_32f(minX, minY, cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.y0_f[segY], temp0, temp1, count);

	nmppsMulC_AddV_32f(temp1, temp0, temp2, segWidth, count);
	nmppsConvert_32f32s_rounding(temp2, poly->pointInImage + poly->count, 0, count);

	nmblas_scopy(count, (float*)lines->z, 1, (float*)poly->z + poly->count, 1);
	nmblas_scopy(4 * count, (float*)lines->colors, 1, (float*)poly->color + 4 * poly->count, 1);

	poly->count += count;
}