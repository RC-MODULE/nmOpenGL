#include "nmtype.h"
#include "demo3d_common.h"

extern "C" SECTION(".text_demo3d")
int copyArraysByMask(float** srcPointers, nm1* mask, float** dstPointers, int nArrays, int size){
	int counter = 0;
	int i = 0;
	int* mask32 = (int*)mask;
	while (size > 0) {
		int localMaskSize = MIN(size, 32);
		int iMask = mask32[i];
		if (iMask) {
			for (int j = 0; j < localMaskSize; j++) {
				if (iMask & 1) {
					int nSrc = i * 32 + j;
					for (int j = 0; j < nArrays; j++) {
						dstPointers[j][counter] = srcPointers[j][nSrc];
					}
					counter++;
				}
				iMask >>= 1;
			}
		}
		i++;
		size -= 32;
	}
	if (counter & 1) {
		for (int j = 0; j < nArrays; j++) {
			dstPointers[j][counter] = dstPointers[j][counter - 1];
		}
		counter++;
	}
	return counter;
}