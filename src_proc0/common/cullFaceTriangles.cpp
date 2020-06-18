#include "nmgl.h"
#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmgl_data0.h"
#include <nmpp.h>

#define COPY_TRIANGLE_IN_PLACE(iSrc, iDst) triangles.x2[iDst] = triangles.x2[iSrc];	\
triangles.y2[iDst] = triangles.y2[iSrc];												\
triangles.x1[iDst] = triangles.x1[iSrc];												\
triangles.y1[iDst] = triangles.y1[iSrc];												\
triangles.x0[iDst] = triangles.x0[iSrc];												\
triangles.y0[iDst] = triangles.y0[iSrc];												\
triangles.z[iDst] = triangles.z[iSrc];												\
colorVec[4 * (iDst) + 0] = colorVec[4 * (iSrc) + 0];						\
colorVec[4 * (iDst) + 1] = colorVec[4 * (iSrc) + 1];						\
colorVec[4 * (iDst) + 2] = colorVec[4 * (iSrc) + 2];						\
colorVec[4 * (iDst) + 3] = colorVec[4 * (iSrc) + 3];

SECTION(".text_demo3d")
void cullFaceSortTriangles(Triangles &triangles){
	int count = triangles.size;
	int* colorVec = (int*)triangles.colors;
	float* walkDirection = cntxt.buffer2 + 6 * NMGL_SIZE;
	float* temp0 = cntxt.buffer0 + 6 * NMGL_SIZE;
	float* temp1 = cntxt.buffer3;
	int* evenMaskVec = cntxt.dividedMasks[0].even.bits;
	int* oddMaskVec= cntxt.dividedMasks[0].odd.bits;

	nmppsMul_Mul_Sub_32f(triangles.x0, triangles.y1, triangles.x1, triangles.y0, walkDirection, count);
	nmppsMul_Mul_Sub_32f(triangles.x1, triangles.y2, triangles.x2, triangles.y1, temp1, count);
	nmppsAdd_32f(walkDirection, temp1, temp0, count);
	nmppsMul_Mul_Sub_32f(triangles.x2, triangles.y0, triangles.x0, triangles.y2, temp1, count);
	nmppsAdd_32f(temp0, temp1, walkDirection, count);

	if (cntxt.frontFaceOrientation == NMGL_CW) {
		nmppsSubCRev_32f(walkDirection, walkDirection, 0, count);
	}

	cntxt.tmp.vec[0] = cntxt.tmp.vec[1] = 0;
	if (cntxt.cullFaceType == NMGL_BACK) {
		nmppsCmpLtC_v2nm32f((v2nm32f*)walkDirection, (v2nm32f*)&cntxt.tmp, evenMaskVec, oddMaskVec, 1, count / 2);
	}
	else {
		nmppsCmpGtC_v2nm32f((v2nm32f*)walkDirection, (v2nm32f*)&cntxt.tmp, evenMaskVec, oddMaskVec, 1, count / 2);
	}

	int resultCounter = 0;
	int size = count / 2;
	int i = 0;
	while (size > 0) {
		int maskEven = evenMaskVec[i];
		int maskOdd  = oddMaskVec[i];
		int localSize = MIN(32, size);
		if (maskEven | maskOdd) {
			for (int j = 0; j < localSize; j++) {
				if (maskEven % 2) {
					COPY_TRIANGLE_IN_PLACE(2 * (i * 32 + j), resultCounter);
					resultCounter++;
				}
				if (maskOdd % 2) {
					COPY_TRIANGLE_IN_PLACE(2 * (i * 32 + j) + 1, resultCounter);
					resultCounter++;
				}
				maskEven >>= 1;
				maskOdd >>= 1;
			}
		}
		size -= 32;
		i++;
	}
	
	while (resultCounter % 2) {
		COPY_TRIANGLE_IN_PLACE(resultCounter - 1, resultCounter);
		resultCounter++;
	}
	triangles.size = resultCounter;
}
