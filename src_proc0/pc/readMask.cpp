#include "nmtype.h"
#include "demo3d_common.h"
#include "stdio.h"

extern "C"{

int readMask(nm1* mask, int* dstIndices, int size) {
	int result = 0;
	int i = 0;
	int i32 = 0;
	int* maskVec = ((int*)mask);

	while (i < size) {
		unsigned int tmpMask = maskVec[i32++];
		unsigned int remainingBits = 32;
		while (tmpMask) {
			if (tmpMask & 1) {
				dstIndices[result++] = i;
			}
			i++;
			tmpMask >>= 1;
			remainingBits--;
		}
		i += remainingBits;
	}
	if (result & 1) {
		dstIndices[result] = dstIndices[result - 1];
		result++;
	}
	return result;
}

}