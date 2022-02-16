/*!
 *  \file cullFaceTriangles.cpp
 *  \author Zhilenkov Ivan
 */

#include "nmpp.h"
#include "context_float.h"
#include "nmgl.h"
#include "nmgl.h"
#include "perfragment.h"
#include "raster.h"

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

__attribute__((section(".text_demo3d")))

int cullFaceSortTriangles(NMGL_CullFace *cullface, TrianglePointers &triangles, int count){
	NMGL_CoreContextFloat *context = getCoreContextFloat();

	float* walkDirection = context->pools[2].f + 6 * context->poolSize32;
	float* temp0 = context->pools[2].f;
	int* temp0i = context->pools[2].i;
	float* temp1 = context->pools[3].f;
	int* evenMaskVec = context->pools[2].i + context->poolSize32;
	int* oddMaskVec= context->pools[3].i + context->poolSize32;

	nmppsMul_Mul_Sub_32f(triangles.v0.x, triangles.v1.y, triangles.v1.x, triangles.v0.y, walkDirection, count);
	nmppsMul_Mul_Sub_32f(triangles.v1.x, triangles.v2.y, triangles.v2.x, triangles.v1.y, temp1, count);
	nmppsAdd_32f(walkDirection, temp1, temp0, count);
	nmppsMul_Mul_Sub_32f(triangles.v2.x, triangles.v0.y, triangles.v0.x, triangles.v2.y, temp1, count);
	nmppsAdd_32f(temp0, temp1, walkDirection, count);

	
	if (cullface->frontface == NMGL_CW) {
		nmppsSubCRev_32f(walkDirection, walkDirection, 0, count);
	}

	context->temp.vec[0] = context->temp.vec[1] = 0;
	if (cullface->type == NMGL_BACK) {
		nmppsCmpLtC_v2nm32f((v2nm32f*)walkDirection, (v2nm32f*)&context->temp, evenMaskVec, oddMaskVec, 1, count / 2);
	}
	else {
		nmppsCmpGtC_v2nm32f((v2nm32f*)walkDirection, (v2nm32f*)&context->temp, evenMaskVec, oddMaskVec, 1, count / 2);
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
