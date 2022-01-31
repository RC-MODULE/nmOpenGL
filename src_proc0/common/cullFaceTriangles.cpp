/*!
 *  \file cullFaceTriangles.cpp
 *  \author Zhilenkov Ivan
 */
#include "nmgl.h"
#include "demo3d_common.h"
#include "demo3d_nm0.h"

#include "nmpp.h"

inline void copyVertex(CombinePointers &vertex, int iSrc, int iDst) {
	vertex.x[iDst] = vertex.x[iSrc];
	vertex.y[iDst] = vertex.y[iSrc];
	vertex.z[iDst] = vertex.z[iSrc];
	vertex.w[iDst] = vertex.w[iSrc];
	vertex.color[iDst] = vertex.color[iSrc];
	//TEXTURING_PART
	//if (cntxt->texState.textureEnabled) {
		vertex.s[iDst] = vertex.s[iSrc];
		vertex.t[iDst] = vertex.t[iSrc];
	//}
	//TEXTURING_PART
}

inline void copyTriangleInPlace(TrianglePointers &triangles, int iSrc, int iDst) {
	copyVertex(triangles.v0, iSrc, iDst);
	copyVertex(triangles.v1, iSrc, iDst);
	copyVertex(triangles.v2, iSrc, iDst);
}
extern "C" int cnvDividedMaskToIndices(nm1* maskEven, nm1* maskOdd, int* indices, int size);

SECTION(".text_demo3d")
int cullFaceSortTriangles(TrianglePointers &triangles, int count){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

	float* walkDirection = cntxt->buffer2 + 6 * NMGL_SIZE;
	float* temp0 = cntxt->buffer2;
	int* temp0i = (int*)cntxt->buffer2;
	float* temp1 = cntxt->buffer3;
	int* evenMaskVec = (int*)cntxt->buffer2 + NMGL_SIZE;
	int* oddMaskVec= (int*)cntxt->buffer3 + NMGL_SIZE;

	nmppsMul_Mul_Sub_32f(triangles.v0.x, triangles.v1.y, triangles.v1.x, triangles.v0.y, walkDirection, count);
	nmppsMul_Mul_Sub_32f(triangles.v1.x, triangles.v2.y, triangles.v2.x, triangles.v1.y, temp1, count);
	nmppsAdd_32f(walkDirection, temp1, temp0, count);
	nmppsMul_Mul_Sub_32f(triangles.v2.x, triangles.v0.y, triangles.v0.x, triangles.v2.y, temp1, count);
	nmppsAdd_32f(temp0, temp1, walkDirection, count);

	if (cntxt->frontFaceOrientation == NMGL_CW) {
		nmppsSubCRev_32f(walkDirection, walkDirection, 0, count);
	}

	cntxt->tmp.vec[0] = cntxt->tmp.vec[1] = 0;
	if (cntxt->cullFaceType == NMGL_BACK) {
		nmppsCmpLtC_v2nm32f((v2nm32f*)walkDirection, (v2nm32f*)&cntxt->tmp, evenMaskVec, oddMaskVec, 1, count / 2);
	}
	else {
		nmppsCmpGtC_v2nm32f((v2nm32f*)walkDirection, (v2nm32f*)&cntxt->tmp, evenMaskVec, oddMaskVec, 1, count / 2);
	}

	count = cnvDividedMaskToIndices(evenMaskVec, oddMaskVec, (int*)temp0i, count);
	int resultCounter = 0;
	for (int i = 0; i < count; i++) {
		copyTriangleInPlace(triangles, temp0i[i], resultCounter++);
	}
	if (resultCounter % 2) {
		copyTriangleInPlace(triangles, resultCounter - 1, resultCounter);
		resultCounter++;
	}

	return resultCounter;
}
