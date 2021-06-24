/*!
 *  \file setSegmentMask.cpp
 *  \author Zhilenkov Ivan
 */
#include "nmpp.h"
#include "demo3d_nm0.h"

#include "nmblas.h"


SECTION(".text_demo3d")
void and4v(int* mask1, int* mask2, int* mask3, int* mask4, int* dst, int size) {
	volatile int a= 0;
	for (int i = 0, cnt = 0; cnt < size; i++, cnt += 32) {
		int result = (mask1[i] & mask2[i]) & (mask3[i] & mask4[i]);
		dst[i] = result;
		a++;
	}
}

int firstNonZeroIndx_32s(int* pSrcVec, int nSize);

SECTION(".text_demo3d")
void setSegmentMask(v2nm32f* minXY, v2nm32f* maxXY, BitMask* masks, int size){
	NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();
	int nSegments = 0;
	v2nm32f* lowerLimit;
	v2nm32f* upperLimit;
	if (!cntxt->scissorTest.isEnabled) {
		nSegments = cntxt->windowInfo.nSegments;
		lowerLimit = cntxt->windowInfo.lowerLeft;
		upperLimit = cntxt->windowInfo.upperRight;
	}
	else {
		nSegments = cntxt->scissorTest.nSegments;
		lowerLimit = cntxt->scissorTest.lowerLeft;
		upperLimit = cntxt->scissorTest.upperRight;
	}

	for (int iSeg = 0; iSeg < nSegments; iSeg++) {

		int* maskXLt = (int*)cntxt->buffer0;
		int* maskYLt = (int*)cntxt->buffer1;
		int* maskXGt = (int*)cntxt->buffer2;
		int* maskYGt = (int*)cntxt->buffer3;

		nmppsCmpLtC_v2nm32f(minXY, upperLimit + iSeg, (nm1*)maskXLt, (nm1*)maskYLt, 1, size);
		nmppsCmpGtC_v2nm32f(maxXY, lowerLimit + iSeg, (nm1*)maskXGt, (nm1*)maskYGt, 1, size);
		//and4v(maskXLt, maskYLt, maskXGt, maskYGt, masks[iSeg].bits, size);
		for (int i = 0, cnt = 0; cnt < size; i++, cnt += 32) {
			int result = (maskXLt[i] & maskYLt[i]) & (maskXGt[i] & maskYGt[i]);
			masks[iSeg].bits[i] = result;
		}
		int size32 = (size + 31) / 32;
		int ind = firstNonZeroIndx_32s(masks[iSeg].bits, size32);
		//если ind = -1, значит все нули и в hasNotZeroBits писать 1, иначе наоборот
		masks[iSeg].hasNotZeroBits = (ind != -1);

	}

}
