/*!
 *  \file pushToTriangles_t.cpp
 *  \brief Файл с функцией pushToTriangles_t
 *  \author Zhilenkov Ivan
 */
#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "utility_float.h"



SECTION(".text_demo3d")
void pushToTriangles(const TrianglePointers &srcTriangles, Triangles& dstTriangles, int countVertex){

	//v2 color is used to implement NMGL_FLAT mode for triangles
	nmppsConvert_32f32s_rounding((nm32f*)srcTriangles.v2.color, (int*)dstTriangles.colors, 0, 4 * countVertex);
	
	nmblas_scopy(countVertex, srcTriangles.v0.x, 1, dstTriangles.x0, 1);
	nmblas_scopy(countVertex, srcTriangles.v0.y, 1, dstTriangles.y0, 1);
	nmblas_scopy(countVertex, srcTriangles.v1.x, 1, dstTriangles.x1, 1);
	nmblas_scopy(countVertex, srcTriangles.v1.y, 1, dstTriangles.y1, 1);
	nmblas_scopy(countVertex, srcTriangles.v2.x, 1, dstTriangles.x2, 1);
	nmblas_scopy(countVertex, srcTriangles.v2.y, 1, dstTriangles.y2, 1);

	// TEXTURING_PART
	nmblas_scopy(countVertex, srcTriangles.v0.w, 1, dstTriangles.w0, 1);
	nmblas_scopy(countVertex, srcTriangles.v1.w, 1, dstTriangles.w1, 1);
	nmblas_scopy(countVertex, srcTriangles.v2.w, 1, dstTriangles.w2, 1);

	nmblas_scopy(countVertex, srcTriangles.v0.s, 1, dstTriangles.s0, 1);
	nmblas_scopy(countVertex, srcTriangles.v0.t, 1, dstTriangles.t0, 1);
	nmblas_scopy(countVertex, srcTriangles.v1.s, 1, dstTriangles.s1, 1);
	nmblas_scopy(countVertex, srcTriangles.v1.t, 1, dstTriangles.t1, 1);
	nmblas_scopy(countVertex, srcTriangles.v2.s, 1, dstTriangles.s2, 1);
	nmblas_scopy(countVertex, srcTriangles.v2.t, 1, dstTriangles.t2, 1);
	// TEXTURING_PART

	meanToInt3(srcTriangles.v0.z, srcTriangles.v1.z, srcTriangles.v2.z, dstTriangles.z, countVertex);
	dstTriangles.size = countVertex;
}
