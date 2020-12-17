/*!
 *  \file pushToLines_l.cpp
 *  \author Zhilenkov Ivan
 */
#include "nmpp.h"
#include "nmblas.h"
#include "demo3d_nm0.h"


#define HEAD_X0 (triangles.x0)
#define HEAD_Y0 (triangles.y0)
#define HEAD_X1 (triangles.x1)
#define HEAD_Y1 (triangles.y1)
#define HEAD_X2 (triangles.x2)
#define HEAD_Y2 (triangles.y2)
#define HEAD_Z  (triangles.z )
#define HEAD_COLOR  (triangles.colors)


SECTION(".text_demo3d")
void pushToLines(const float *vertexX, const float *vertexY, const float *vertexZ, const v4nm32f* color, Lines& lines, int mode, int countVertex){
	int countPrim = 0;
	float* srcColor = (float*)color;
	int* dstColorLine = (int*)lines.colors;
	
	switch(mode){
	case NMGL_LINES:
		countPrim = countVertex / 2;
		for(int i = 0; i < countPrim; i++){
			lines.x0[i] = vertexX[2 * i + 0];
			lines.y0[i] = vertexY[2 * i + 0];
			lines.x1[i] = vertexX[2 * i + 1];
			lines.y1[i] = vertexY[2 * i + 1];
			lines.z[i] = (vertexZ[2 * i] + vertexZ[2 * i + 1]) / 2;
			dstColorLine[4 * i + 0] = (int)srcColor[2 * 4 * i + 0];
			dstColorLine[4 * i + 1] = (int)srcColor[2 * 4 * i + 1];
			dstColorLine[4 * i + 2] = (int)srcColor[2 * 4 * i + 2];
			dstColorLine[4 * i + 3] = (int)srcColor[2 * 4 * i + 3];
		}
		break;
	case NMGL_LINE_STRIP:
	case NMGL_LINE_LOOP:
		countPrim = countVertex - 1;
		for (int i = 1; i < countPrim; i++) {
			lines.x0[i] = vertexX[i - 1];
			lines.y0[i] = vertexY[i - 1];
			lines.x1[i] = vertexX[i];
			lines.y1[i] = vertexY[i];
			lines.z[i] = (vertexZ[i] + vertexZ[i - 1]) / 2;
			dstColorLine[4 * i + 0] = (int)srcColor[4 * i + 0];
			dstColorLine[4 * i + 1] = (int)srcColor[4 * i + 1];
			dstColorLine[4 * i + 2] = (int)srcColor[4 * i + 2];
			dstColorLine[4 * i + 3] = (int)srcColor[4 * i + 3];
		}
		if (mode == NMGL_LINE_LOOP) {
			lines.x0[countPrim] = vertexX[countPrim - 1];
			lines.x1[countPrim] = vertexX[0];
			lines.y0[countPrim] = vertexY[countPrim - 1];
			lines.y1[countPrim] = vertexY[0];
			lines.z[countPrim] = (vertexZ[countPrim - 1] + vertexZ[0]) / 2;
			dstColorLine[4 * countPrim + 0] = (int)srcColor[0];
			dstColorLine[4 * countPrim + 1] = (int)srcColor[1];
			dstColorLine[4 * countPrim + 2] = (int)srcColor[2];
			dstColorLine[4 * countPrim + 3] = (int)srcColor[3];
			countPrim++;
		}		
		break;
	}
	if (countPrim % 2) {
		lines.x0[countPrim] = lines.x0[countPrim - 1];
		lines.x1[countPrim] = lines.x1[countPrim - 1];
		lines.y0[countPrim] = lines.y0[countPrim - 1];
		lines.y1[countPrim] = lines.y1[countPrim - 1];
		lines.z[countPrim] = lines.z[countPrim - 1];
		dstColorLine[4 * countPrim + 0] = dstColorLine[4 * (countPrim - 1) + 0];
		dstColorLine[4 * countPrim + 1] = dstColorLine[4 * (countPrim - 1) + 1];
		dstColorLine[4 * countPrim + 2] = dstColorLine[4 * (countPrim - 1) + 2];
		dstColorLine[4 * countPrim + 3] = dstColorLine[4 * (countPrim - 1) + 3];
		countPrim++;
	}

	lines.size = countPrim;
}