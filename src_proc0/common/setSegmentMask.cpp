#include "nmpp.h"
#include "demo3d_common.h"
#include "nmgl_data0.h"
#include "nmblas.h"

SECTION(".text_demo3d")
void setSegmentMask(NMGL_Context_NM0 &cntxt, v2nm32f* minXY, v2nm32f* maxXY, BitMask* masks, int size){
	for (int segY = 0, iSeg = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++, iSeg++) {
			v2nm32f* lowerLimit = (v2nm32f*)&cntxt.tmp;
			v2nm32f* upperLimit = (v2nm32f*)&cntxt.tmp.vec[2];
			lowerLimit->v0 = cntxt.windowInfo.x0_f[segX];
			lowerLimit->v1 = cntxt.windowInfo.y0_f[segY];
			upperLimit->v0 = cntxt.windowInfo.x1_f[segX];
			upperLimit->v1 = cntxt.windowInfo.y1_f[segY];

			int* maskXLt = (int*)cntxt.dividedMasks[0].even.bits;
			int* maskYLt = (int*)cntxt.dividedMasks[0].odd.bits;
			int* maskXGt = (int*)cntxt.dividedMasks[1].even.bits;
			int* maskYGt = (int*)cntxt.dividedMasks[1].odd.bits;
			nmppsCmpLtC_v2nm32f((v2nm32f*)minXY, upperLimit, (nm1*)maskXLt, (nm1*)maskYLt, 1, size);
			nmppsCmpGtC_v2nm32f((v2nm32f*)maxXY, lowerLimit, (nm1*)maskXGt, (nm1*)maskYGt, 1, size);
			for (int i = 0, cnt = 0; cnt < size; i++, cnt += 32) {
				int result = (maskXLt[i] & maskYLt[i]) & (maskXGt[i] & maskYGt[i]);
				masks[iSeg].bits[i] = result;
			}
			int size32 = MIN(NMGL_SIZE / 32, size / 32 + 2);
			if (firstNonZeroIndx_32s(masks[iSeg].bits, size32) >= 0) {
				masks[iSeg].hasNotZeroBits = 1;
			}
			else {
				masks[iSeg].hasNotZeroBits = 0;
			}
		}
	}
}