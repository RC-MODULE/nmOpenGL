/*!
 *  \file pushToLines_l.cpp
 *  \author Zhilenkov Ivan
 */
#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"


SECTION(".text_demo3d")
void pushToLines(const LinePointers &srcLines, Lines &dstLines, int countVertex){
	nmppsConvert_32f32s_rounding((nm32f*)srcLines.v0.color, (int*)dstLines.colors, 0, 4 * countVertex);

	nmblas_scopy(countVertex, srcLines.v0.x, 1, dstLines.x0, 1);
	nmblas_scopy(countVertex, srcLines.v0.y, 1, dstLines.y0, 1);
	nmblas_scopy(countVertex, srcLines.v1.x, 1, dstLines.x1, 1);
	nmblas_scopy(countVertex, srcLines.v1.y, 1, dstLines.y1, 1);

#ifdef TEXTURE_ENABLED
	nmblas_scopy(countVertex, srcLines.v0.w, 1, dstLines.w0, 1);
	nmblas_scopy(countVertex, srcLines.v1.w, 1, dstLines.w1, 1);

	nmblas_scopy(countVertex, srcLines.v0.s, 1, dstLines.s0, 1);
	nmblas_scopy(countVertex, srcLines.v0.t, 1, dstLines.t0, 1);
	nmblas_scopy(countVertex, srcLines.v1.s, 1, dstLines.s1, 1);
	nmblas_scopy(countVertex, srcLines.v1.t, 1, dstLines.t1, 1);
#endif

	meanToInt2(srcLines.v0.z, srcLines.v1.z, dstLines.z, countVertex);
	dstLines.size = countVertex;
}