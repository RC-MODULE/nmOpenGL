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
void pushToLines_l(const float *vertexX, const float *vertexY, const float *vertexZ, const v4nm32f* color, Lines& lines, int countVertex){
	int countPrim = countVertex / 2;
	int* colorInt = (int*)color;
	int* colorIntLine = (int*)lines.colors;
	
	for(int i = 0; i < countPrim; i++){
		lines.x0[i] = vertexX[2 * i + 0];
		lines.x1[i] = vertexX[2 * i + 1];
		lines.y0[i] = vertexY[2 * i + 0];
		lines.y1[i] = vertexY[2 * i + 1];
		lines.z[i] = (vertexZ[2 * i] + vertexZ[2 * i + 1]) / 2;
		colorIntLine[4 * i + 0] = colorInt[2 * 4 * i + 0];
		colorIntLine[4 * i + 1] = colorInt[2 * 4 * i + 1];
		colorIntLine[4 * i + 2] = colorInt[2 * 4 * i + 2];
		colorIntLine[4 * i + 3] = colorInt[2 * 4 * i + 3];
	}

	lines.size = countPrim;
}