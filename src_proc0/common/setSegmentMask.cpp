#include "nmpp.h"
#include "demo3d_common.h"
#include "nmgl_data0.h"
#include "stdio.h"

SECTION(".text_demo3d")
void setSegmentMask(v2nm32f* minXY, v2nm32f* maxXY, SegmentMask* masks, int primCount) {
	for (int segY = 0, iSeg = 0; segY < cntxt.windowInfo.nRows; segY++) {
		for (int segX = 0; segX < cntxt.windowInfo.nColumns; segX++, iSeg++) {
			v2nm32f* lowerLimit = (v2nm32f*)&cntxt.tmp;
			v2nm32f* upperLimit = (v2nm32f*)&cntxt.tmp.vec[2];
			lowerLimit->v0 = cntxt.windowInfo.x0_f[segX];
			lowerLimit->v1 = cntxt.windowInfo.y0_f[segY];
			upperLimit->v0 = cntxt.windowInfo.x1_f[segX];
			upperLimit->v1 = cntxt.windowInfo.y1_f[segY];

			int* maskXLt = (int*)cntxt.buffer0;
			int* maskYLt = (int*)cntxt.buffer1;
			int* maskXGt = (int*)cntxt.buffer2;
			int* maskYGt = (int*)cntxt.buffer3;
			nmppsCmpLtC_v2nm32f(minXY, upperLimit, (nm1*)maskXLt, (nm1*)maskYLt, 1, primCount);
			nmppsCmpGtC_v2nm32f(maxXY, lowerLimit, (nm1*)maskXGt, (nm1*)maskYGt, 1, primCount);
			int i = 0;
			int tmpCount = primCount;
			while (tmpCount > 0) {
				masks[iSeg].bits[i] = (maskXLt[i] & maskYLt[i]) & (maskXGt[i] & maskYGt[i]);
				masks[iSeg].hasNotZeroBits |= masks[iSeg].bits[i];
				i++;
				tmpCount -= 32;
			}
		}
	}
}