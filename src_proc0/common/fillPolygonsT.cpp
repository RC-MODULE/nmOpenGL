#include "demo3d_nm0.h"
#include "nmpp.h"
#include "nmgl_data0.h"

SECTION(".text_demo3d")
void fillPolygonsT(Polygons* poly, Triangles* triangles, int count, int segX, int segY){
	poly->segX0 = cntxt.windowInfo.x0[segX];
	poly->segY0 = cntxt.windowInfo.y0[segY];
	poly->segWidth = cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX];
	poly->segHeight = cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY];

	float* pAxy = cntxt.buffer1 + 6 * NMGL_SIZE;
	float* pBxy = cntxt.buffer2 + 6 * NMGL_SIZE;
	float* pCxy = cntxt.buffer3 + 6 * NMGL_SIZE;
	nmppsMerge_32f(triangles->x0, triangles->y0, pCxy, count);
	nmppsMerge_32f(triangles->x1, triangles->y1, pBxy, count);
	nmppsMerge_32f(triangles->x2, triangles->y2, pAxy, count);
	sortByY3(pCxy, pBxy, pAxy, count);
	split_v2nm32f((v2nm32f*)pCxy, 1, triangles->x0, triangles->y0, count);
	split_v2nm32f((v2nm32f*)pBxy, 1, triangles->x1, triangles->y1, count);
	split_v2nm32f((v2nm32f*)pAxy, 1, triangles->x2, triangles->y2, count);

	float* minX = cntxt.buffer1 + 6 * NMGL_SIZE;
	float* maxX = cntxt.buffer2 + 6 * NMGL_SIZE;
	float* temp = cntxt.buffer3 + 6 * NMGL_SIZE;
	findMinMax3(triangles->x2, triangles->x1, triangles->x0, minX, maxX, count);

	nmppsSubC_32f(triangles->x0, temp, cntxt.windowInfo.x0_f[segX], count);
	nmppsConvert_32f32s_rounding(temp, poly->x0, 0, count);

	nmppsSubC_32f(triangles->y0, temp, cntxt.windowInfo.y0_f[segY], count);
	nmppsConvert_32f32s_rounding(temp, poly->y0, 0, count);

	nmppsSubC_32f(minX, temp, cntxt.windowInfo.x0_f[segX], count);
	nmppsConvert_32f32s_rounding(temp, poly->minX, 0, count);

	nmppsSub_32f(maxX, minX, temp, count);
	nmppsConvert_32f32s_rounding(temp, poly->maxDiffX, 0, count);

	float* dx1A_float = cntxt.buffer0 + 7 * NMGL_SIZE;
	float* dy1A_float = cntxt.buffer1 + 7 * NMGL_SIZE;
	nmppsSub_32f(triangles->x2, triangles->x1, dx1A_float, count);
	nmppsSub_32f(triangles->y2, triangles->y1, dy1A_float, count);
	nmppsConvert_32f32s_rounding(dx1A_float, poly->dx12, 0, count);
	nmppsConvert_32f32s_rounding(dy1A_float, poly->dy12, 0, count);

	float* dxCA_float = cntxt.buffer2 + 6 * NMGL_SIZE;
	float* dyCA_float = cntxt.buffer3 + 6 * NMGL_SIZE;
	nmppsSub_32f(triangles->x2, triangles->x0, dxCA_float, count);
	nmppsSub_32f(triangles->y2, triangles->y0, dyCA_float, count);
	nmppsConvert_32f32s_rounding(dxCA_float, poly->dx02, 0, count);
	nmppsConvert_32f32s_rounding(dyCA_float, poly->dy02, 0, count);

	float* dxCB_float = cntxt.buffer2 + 8 * NMGL_SIZE;
	float* dyCB_float = cntxt.buffer3 + 8 * NMGL_SIZE;
	nmppsSub_32f(triangles->x1, triangles->x0, dxCB_float, count);
	nmppsSub_32f(triangles->y1, triangles->y0, dyCB_float, count);
	nmppsConvert_32f32s_rounding(dxCB_float, poly->dx01, 0, count);
	nmppsConvert_32f32s_rounding(dyCB_float, poly->dy01, 0, count);

	nmppsMul_Mul_Sub_32f(dxCA_float, dyCB_float, dyCA_float, dxCB_float, (float*)poly->crossProduct, count);

	poly->count = count;
}