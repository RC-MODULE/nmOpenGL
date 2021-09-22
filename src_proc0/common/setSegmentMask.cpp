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

	int nSegments = cntxt->currentSegments->count;
	v2nm32f* lowerLimit = cntxt->currentSegments->lowerLeft;
	v2nm32f* upperLimit = cntxt->currentSegments->upperRight;

	for (int iSeg = 0; iSeg < nSegments; iSeg++) {
		checkRectanglesOverlaps(minXY, maxXY, upperLimit + iSeg, lowerLimit + iSeg, masks[iSeg].bits, size);
		int size32 = (size + 31) / 32;
		int ind = firstNonZeroIndx_32s(masks[iSeg].bits, size32);
		//если ind = -1, значит все нули и в hasNotZeroBits писать 1, иначе наоборот
		masks[iSeg].hasNotZeroBits = (ind != -1);

	}

}
