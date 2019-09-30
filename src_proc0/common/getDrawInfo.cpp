#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmgl_data0.h"
#include "nmpp.h"

inline void getNumbersPtrns(float* x0, float* y0, float* x1, float* y1, float* minX, int* dst, int count) {
	static float* temp0 = cntxt.buffer0 + 2 * NMGL_SIZE;
	static float* temp1 = cntxt.buffer1 + 2 * NMGL_SIZE;
	static float* temp2 = cntxt.buffer2 + 2 * NMGL_SIZE;
	static float* temp3 = cntxt.buffer3;

	nmppsSub_32f(x1, x0, temp0, count);		//dx
	nmppsSub_32f(y1, y0, temp1, count);		//dy
	nmppsMulC_Add_32fcr((nm32fcr*)temp0, (nm32fcr*)temp1, (nm32fcr*)temp3, 2 * WIDTH_PTRN, count / 2);		//i
	nmppsConvert_32f32s_rounding(temp3, (int*)temp0, 0, count);
	remap_32u((nm32u*)cntxt.patterns->table_dydx, (nm32u*)temp1, (nm32s*)temp0, count);
	nmppsConvert_32s32f((nm32s*)temp1, temp0, count);
	nmppsSub_32f(x0, minX, temp2, count);				//x0
	nmppsAdd_32f(temp0, temp2, temp1, count);					//dydx(dx, dy) + x0
	nmppsConvert_32f32s_rounding(temp1, dst, 0, count);
}

inline void getCrossProducts(Triangles* triangles, int* dst, int count) {
	static float* dxCA_float = cntxt.buffer2 + NMGL_SIZE;
	static float* dyCA_float = cntxt.buffer3 + NMGL_SIZE;
	static float* dxCB_float = cntxt.buffer2 + 2 * NMGL_SIZE;
	static float* dyCB_float = cntxt.buffer3 + 2 * NMGL_SIZE;

	nmppsSub_32f(triangles->x2, triangles->x0, dxCA_float, count);
	nmppsSub_32f(triangles->y2, triangles->y0, dyCA_float, count);

	nmppsSub_32f(triangles->x1, triangles->x0, dxCB_float, count);
	nmppsSub_32f(triangles->y1, triangles->y0, dyCB_float, count);

	nmppsMul_Mul_Sub_32f(dxCA_float, dyCB_float, dyCA_float, dxCB_float, (float*)dst, count);
}


void getDrawInfo(Triangles* triangles, DrawInfo* drawInfo, int count, int segX, int segY){
	Patterns* patterns = cntxt.patterns;

	//int segX0 = cntxt.windowInfo.x0[segX];
	//int segY0 = cntxt.windowInfo.y0[segY];
	//int segWidth = cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX];
	//int segHeight = cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY];

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

	nmppsSubC_32f(triangles->x0, triangles->x0, cntxt.windowInfo.x0_f[segX], count);
	nmppsSubC_32f(triangles->x1, triangles->x1, cntxt.windowInfo.x0_f[segX], count);
	nmppsSubC_32f(triangles->x2, triangles->x2, cntxt.windowInfo.x0_f[segX], count);

	float* minX = cntxt.buffer1 + 6 * NMGL_SIZE;
	float* maxX = cntxt.buffer2 + 6 * NMGL_SIZE;
	float* temp = cntxt.buffer3 + 6 * NMGL_SIZE;
	findMinMax3(triangles->x0, triangles->x1, triangles->x2, minX, maxX, count);	

	getNumbersPtrns(triangles->x0, triangles->y0, triangles->x1, triangles->y1, minX, drawInfo->numbersPattrns01, count);
	getNumbersPtrns(triangles->x0, triangles->y0, triangles->x2, triangles->y2, minX, drawInfo->numbersPattrns02, count);
	getNumbersPtrns(triangles->x1, triangles->y1, triangles->x2, triangles->y2, minX, drawInfo->numbersPattrns12, count);
	
	getCrossProducts(triangles, drawInfo->crossProducts, count);

	float* temp0 = cntxt.buffer0 + 2 * NMGL_SIZE;
	float* temp1 = cntxt.buffer1 + 2 * NMGL_SIZE;
	
	absIfNegElse0_32f(triangles->y0, temp0, count);
	nmppsMulC_32f(temp0, temp1, WIDTH_PTRN/16, count);
	nmppsConvert_32f32s_rounding(temp1, drawInfo->offsetTrY, 0, count);

	absIfNegElse0_32f(minX, temp0, count);
	nmppsConvert_32f32s_rounding(temp0, drawInfo->offsetTrX, 0, count);

	clamp_32f(minX, cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.x1_f[segX], temp0, count);
	clamp_32f(maxX, cntxt.windowInfo.x0_f[segX], cntxt.windowInfo.x1_f[segX], temp1, count);
	nmppsSub_32f(temp1, temp0, cntxt.buffer3, count);
	nmppsConvert_32f32s_rounding(cntxt.buffer3, drawInfo->widths, 0, count);

	clamp_32f(triangles->y0, cntxt.windowInfo.y0_f[segX], cntxt.windowInfo.y1_f[segX], temp0, count);
	clamp_32f(triangles->y2, cntxt.windowInfo.y0_f[segX], cntxt.windowInfo.y1_f[segX], temp1, count);
	nmppsSub_32f(temp1, temp0, cntxt.buffer3, count);
	nmppsConvert_32f32s_rounding(cntxt.buffer3, drawInfo->heights, 0, count);

	nmppsSubC_32f(triangles->y0, temp0, cntxt.windowInfo.y0_f[segX], count);
	nmppsMulC_Add_32fcr((nm32fcr*)temp0, (nm32fcr*)minX, (nm32fcr*)temp1, WIDTH_SEG, count/2);
	nmppsConvert_32f32s_rounding(temp1, drawInfo->imageOffsets, 0, count);

}
