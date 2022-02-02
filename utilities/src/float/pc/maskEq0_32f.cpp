#include "nmtype.h"
extern "C" {
	void maskEq0(v4nm32f* srcVec, v2nm32f* srcMask, v4nm32f* dstVec, int size) {
		for (int i = 0; i < size; i++) {
			if (srcMask[i].v0 == 0) {
				dstVec[i].vec[0] = 0;
				dstVec[i].vec[1] = 0;
				dstVec[i].vec[2] = 0;
				dstVec[i].vec[3] = 0;
			}
			else {
				dstVec[i].vec[0] = srcVec[i].vec[0];
				dstVec[i].vec[1] = srcVec[i].vec[1];
				dstVec[i].vec[2] = srcVec[i].vec[2];
				dstVec[i].vec[3] = srcVec[i].vec[3];
			}
		}
	}
}