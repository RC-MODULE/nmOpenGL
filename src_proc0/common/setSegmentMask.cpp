#include "nmpp.h"
#include "demo3d_common.h"
#include "nmgl_data0.h"
#include "nmblas.h"

SECTION(".data_imu6") int maskTmp[BIG_NMGL_SIZE / 32];

SECTION(".text_demo3d")
void setSegmentMask(NMGL_Context_NM0 &cntxt, SegmentMask* masks) {

	int step = NMGL_SIZE;
	for (int tail = 0; tail < cntxt.trianDdr.size; tail += step) {
		int localSize = MIN(step, cntxt.trianDdr.size - tail);
		nmblas_scopy(localSize, &cntxt.trianDdr.x0[tail], 1, cntxt.trianInner.x0, 1);
		nmblas_scopy(localSize, &cntxt.trianDdr.y0[tail], 1, cntxt.trianInner.y0, 1);
		nmblas_scopy(localSize, &cntxt.trianDdr.x1[tail], 1, cntxt.trianInner.x1, 1);
		nmblas_scopy(localSize, &cntxt.trianDdr.y1[tail], 1, cntxt.trianInner.y1, 1);
		nmblas_scopy(localSize, &cntxt.trianDdr.x2[tail], 1, cntxt.trianInner.x2, 1);
		nmblas_scopy(localSize, &cntxt.trianDdr.y2[tail], 1, cntxt.trianInner.y2, 1);

		float* minXY = cntxt.buffer2 + 6 * NMGL_SIZE;
		float* maxXY = cntxt.buffer3 + 6 * NMGL_SIZE;
		findMinMax3(cntxt.trianInner.x0, cntxt.trianInner.x1, cntxt.trianInner.x2, cntxt.buffer0, cntxt.buffer1, localSize);
		findMinMax3(cntxt.trianInner.y0, cntxt.trianInner.y1, cntxt.trianInner.y2, cntxt.buffer2, cntxt.buffer3, localSize);
		nmppsMerge_32f(cntxt.buffer0, cntxt.buffer2, minXY, localSize);
		nmppsMerge_32f(cntxt.buffer1, cntxt.buffer3, maxXY, localSize);
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
				nmppsCmpLtC_v2nm32f((v2nm32f*)minXY, upperLimit, (nm1*)maskXLt, (nm1*)maskYLt, 1, localSize);
				nmppsCmpGtC_v2nm32f((v2nm32f*)maxXY, lowerLimit, (nm1*)maskXGt, (nm1*)maskYGt, 1, localSize);
				for (int i = 0, cnt = 0; cnt < localSize; i++, cnt += 32) {
					int result = (maskXLt[i] & maskYLt[i]) & (maskXGt[i] & maskYGt[i]);
					maskTmp[i] = result;
				}
				int size32 = MIN(NMGL_SIZE / 32, localSize / 32 + 2);
				nmblas_scopy(size32, (float*)maskTmp, 1, (float*)masks[iSeg].bits + tail / 32, 1);
			}
		}
	}

	int size32 = MIN(BIG_NMGL_SIZE / 32, cntxt.trianDdr.size / 32 + 2);
	for (int iSeg = 0; iSeg < 36; iSeg++) {
		nmblas_scopy(size32, (float*)masks[iSeg].bits, 1, (float*)cntxt.buffer0, 1);
		if (firstNonZeroIndx_32s((int*)cntxt.buffer0, size32) >= 0) {
			masks[iSeg].hasNotZeroBits = 1;
		}
		else {
			masks[iSeg].hasNotZeroBits = 0;
		}
	}
}