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

#ifdef TEXTURE_ENABLED
#define HEAD_S0 (triangles.s0)
#define HEAD_T0 (triangles.t0)
#define HEAD_S1 (triangles.s1)
#define HEAD_T1 (triangles.t1)
#define HEAD_S2 (triangles.s2)
#define HEAD_T2 (triangles.t2)
#define HEAD_Z_EYE0  (triangles.zEye0 )
#define HEAD_Z_EYE1  (triangles.zEye1 )
#define HEAD_Z_EYE2  (triangles.zEye2 )
#endif //TEXTURE_ENABLED


SECTION(".text_demo3d")
#ifdef TEXTURE_ENABLED
void pushToTriangles_t(const float *vertexX, const float *vertexY, const float *vertexZ, 
					   const float *vertexS, const float *vertexT, const float *vertexZEye, 
					   const v4nm32f* color, Triangles& triangles, int countVertex){
#else //TEXTURE_ENABLED
void pushToTriangles_t(const float *vertexX, const float *vertexY, const float *vertexZ, 
					   const v4nm32f* color, Triangles& triangles, int countVertex){
#endif //TEXTURE_ENABLED

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

#ifdef TEXTURE_ENABLED
	if (cntxt.texState.textureEnabled){
		cnv32f_v3v4(vertexS, cntxt.buffer3, 0, countVertex);
		split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, HEAD_S0, HEAD_S1, HEAD_S2, cntxt.buffer3 + 6 * NMGL_SIZE, countPrim);
		
		cnv32f_v3v4(vertexT, cntxt.buffer3, 0, countVertex);
		split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, HEAD_T0, HEAD_T1, HEAD_T2, cntxt.buffer3 + 6 * NMGL_SIZE, countPrim);
		
		cnv32f_v3v4(vertexZEye, cntxt.buffer3, 0, countVertex);
		split_v4nm32f((v4nm32f*)cntxt.buffer3, 1, HEAD_Z_EYE0, HEAD_Z_EYE1, HEAD_Z_EYE2, cntxt.buffer3 + 6 * NMGL_SIZE, countPrim);
    }
#endif //TEXTURE_ENABLED

	triangles.size = countPrim;
}
