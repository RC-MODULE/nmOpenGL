#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmgl_data0.h"
#include "nmpp.h"

void getDrawInfo(Triangles* triangles, DrawInfo* drawInfo, int count, int segX, int segY){
	Patterns* patterns = cntxt.patterns;

	int segX0 = cntxt.windowInfo.x0[segX];
	int segY0 = cntxt.windowInfo.y0[segY];
	int segWidth = cntxt.windowInfo.x1[segX] - cntxt.windowInfo.x0[segX];
	int segHeight = cntxt.windowInfo.y1[segY] - cntxt.windowInfo.y0[segY];

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

	float* temp0 = cntxt.buffer0 + 2 * NMGL_SIZE;
	float* temp1 = cntxt.buffer1 + 2 * NMGL_SIZE;
	float* temp2 = cntxt.buffer2 + 2 * NMGL_SIZE;
	float* temp3 = cntxt.buffer3;
	nmppsSub_32f(triangles->x1, triangles->x0, temp0, count);
	nmppsSub_32f(triangles->y1, triangles->y0, temp1, count);
	nmppsMulC_Add_32fcr((nm32fcr*)temp0, (nm32fcr*)temp1, (nm32fcr*)temp2, 2 * WIDTH_PTRN, count / 2);
	nmppsConvert_32f32s_rounding(temp2, (int*)temp3, 0, count);

	//nmppsRemap_32u((nm32u*)patterns->table_dydx, (nm32u*)pAddrPack, pTmp, nSize);
	//nmppsAdd_32s((nm32s*)pAddrPack, x0, pTmp, nSize);
}
