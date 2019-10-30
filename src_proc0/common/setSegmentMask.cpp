#include "nmpp.h"
#include "demo3d_common.h"
#include "nmgl_data0.h"
#include "nmblas.h"

SECTION(".data_imu6") int maskTmp[BIG_NMGL_SIZE / 32];


SECTION(".text_demo3d")
void setSegmentMask(const v2nm32f* minXY, const v2nm32f* maxXY, SegmentMask* masks, int primCount) {
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
			int haveFillBit = masks[iSeg].hasNotZeroBits;
			nmppsCmpLtC_v2nm32f(minXY, upperLimit, (nm1*)maskXLt, (nm1*)maskYLt, 1, primCount);
			nmppsCmpGtC_v2nm32f(maxXY, lowerLimit, (nm1*)maskXGt, (nm1*)maskYGt, 1, primCount);
			for (int i = 0, cnt = 0; cnt < primCount; i++, cnt += 32) {
				int result = (maskXLt[i] & maskYLt[i]) & (maskXGt[i] & maskYGt[i]);
				result = (maskXLt[i] & maskYLt[i]) & (maskXGt[i] & maskYGt[i]);
				haveFillBit |= result;
				maskTmp[i] = result;
			}
			masks[iSeg].hasNotZeroBits = haveFillBit;
			int size32 = MIN(NMGL_SIZE / 32, primCount / 32 + 2);
			nmblas_scopy(size32, (float*)maskTmp, 1, (float*)masks[iSeg].bits, 1);
		}
	}
}