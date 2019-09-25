#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmgl_data0.h"

SECTION(".text_demo3d")
void fillPolygonsL(Polygons* poly, Lines* lines, int count, int segX, int segY){
	poly->segX0 = cntxt.windowInfo.x0[segX];
	poly->segY0 = cntxt.windowInfo.y0[segY];
	poly->segWidth = cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX];
	poly->segHeight = cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY];

	float* pXY0 = cntxt.buffer1 + 6 * NMGL_SIZE;
	float* pXY1 = cntxt.buffer2 + 6 * NMGL_SIZE;
	nmppsMerge_32f(lines->x0, lines->y0, pXY0, count);
	nmppsMerge_32f(lines->x1, lines->y1, pXY1, count);
	sortByY2(pXY0, pXY1, count);
	split_v2nm32f((v2nm32f*)pXY0, 1, lines->x0, lines->y0, count);
	split_v2nm32f((v2nm32f*)pXY1, 1, lines->x1, lines->y1, count);

	float* minX = cntxt.buffer1 + 6 * NMGL_SIZE;
	float* maxX = cntxt.buffer2 + 6 * NMGL_SIZE;
	float* temp = cntxt.buffer3 + 6 * NMGL_SIZE;
	findMinMax2(lines->x0, lines->x1, minX, maxX, count);

	nmppsSubC_32f(lines->x0, temp, cntxt.windowInfo.x0_f[segX], count);
	nmppsConvert_32f32s_rounding(temp, poly->x0, 0, count);

	nmppsSubC_32f(lines->y0, temp, cntxt.windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_rounding(temp, poly->y0, 0, count);

	nmppsSubC_32f(minX, temp, cntxt.windowInfo.x0_f[segX], count);
	nmppsConvert_32f32s_rounding(temp, poly->minX, 0, count);

	nmppsSub_32f(maxX, minX, temp, count);
	nmppsConvert_32f32s_rounding(temp, poly->maxDiffX, 0, count);

	float* dx01_float = cntxt.buffer2 + 8 * NMGL_SIZE;
	float* dy01_float = cntxt.buffer3 + 8 * NMGL_SIZE;
	nmppsSub_32f(lines->x1, lines->x0, dx01_float, count);
	nmppsSub_32f(lines->y1, lines->y0, dy01_float, count);
	nmppsConvert_32f32s_rounding(dx01_float, poly->dx01, 0, count);
	nmppsConvert_32f32s_rounding(dy01_float, poly->dy01, 0, count);

	poly->count = count;
}