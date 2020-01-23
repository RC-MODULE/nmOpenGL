#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgl_data0.h"

#define HEAD_X0 (triangles.x0)
#define HEAD_Y0 (triangles.y0)
#define HEAD_X1 (triangles.x1)
#define HEAD_Y1 (triangles.y1)
#define HEAD_X2 (triangles.x2)
#define HEAD_Y2 (triangles.y2)
#define HEAD_Z  (triangles.z )
#define HEAD_COLOR  (triangles.colors)


SECTION(".text_demo3d")
void pushToTriangles_t(const float *vertexX, const float *vertexY, const float *vertexZ, const v4nm32f* color, Triangles& triangles, int countVertex){
	int countPrim = countVertex / 3;
	float* temp0 = cntxt.buffer0 + 3 * NMGL_SIZE;
	float* temp1 = cntxt.buffer1 + 6 * NMGL_SIZE;
	float* temp2 = cntxt.buffer2 + 3 * NMGL_SIZE;

	nmblas_dcopy(countPrim, (double*)color, 6, (double*)cntxt.buffer3, 2);
	nmblas_dcopy(countPrim, (double*)color + 1, 6, (double*)cntxt.buffer3 + 1, 2);
	nmppsConvert_32f32s_rounding(cntxt.buffer3, (int*)HEAD_COLOR, 0, 4 * countPrim);

	cnv32f_v3v4(vertexX, cntxt.buffer3, 0, countVertex);
	split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, HEAD_X0, HEAD_X1, HEAD_X2, cntxt.buffer3 + 6 * NMGL_SIZE, countPrim);

	cnv32f_v3v4(vertexY, cntxt.buffer3, 0, countVertex);
	split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, HEAD_Y0, HEAD_Y1, HEAD_Y2, cntxt.buffer3 + 6 * NMGL_SIZE, countPrim);

	cnv32f_v3v4(vertexZ, cntxt.buffer3, 0, countVertex);
	split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, temp0, temp1, temp2, cntxt.buffer3 + 6 * NMGL_SIZE, countPrim);
	meanToInt3(temp0, temp1, temp2, HEAD_Z, countPrim);
	triangles.size = countPrim;
}