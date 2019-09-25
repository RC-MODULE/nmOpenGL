#include "demo3d_common.h"

extern "C" {
int maskSelectionLight_RGBA_BGRA(v4nm32s* srcLight, nm1* mask, v4nm32s* dstLight, int size){
	int counter = 0;
	int i = 0;
	int* mask32 = (int*)mask;
	int* src = (int*)srcLight;
	int* dst = (int*)dstLight;
	while (size > 0) {
		int localMaskSize = MIN(size, 32);
		int iMask = mask32[i];
		if (iMask) {
			for (int j = 0; j < localMaskSize; j++) {
				if (iMask & 1) {
					int nSrc = i * 32 + j;
					dst[4 * counter + 0] = src[4 * nSrc + 2];
					dst[4 * counter + 1] = src[4 * nSrc + 1];
					dst[4 * counter + 2] = src[4 * nSrc + 0];
					dst[4 * counter + 3] = src[4 * nSrc + 3];
					counter++;
				}
				iMask >>= 1;
			}
		}
		i++;
		size -= 32;
	}
	if (counter & 1) {
		dst[4 * counter + 0] = dst[4 * (counter - 1) + 0];
		dst[4 * counter + 1] = dst[4 * (counter - 1) + 1];
		dst[4 * counter + 2] = dst[4 * (counter - 1) + 2];
		dst[4 * counter + 3] = dst[4 * (counter - 1) + 3];
		counter++;
	}
	return counter;
}
}