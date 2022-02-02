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
	while (result % 2) {
		dstIndices[result] = dstIndices[result - 1];
		result++;
	}
	return result;
}

int readDividedMask(nm1* maskEven, nm1* maskOdd, int* dstIndices, int size) {
	int result = 0;
	int i = 0;
	int i32 = 0;
	int* maskVecEven = ((int*)maskEven);
	int* maskVecOdd = ((int*)maskOdd);
	size /= 2;
	while (i < size) {
		unsigned int tmpMaskEven = maskVecEven[i32];
		unsigned int tmpMaskOdd = maskVecOdd[i32];
		i32++;
		unsigned int remainingBits = 32;
		while (tmpMaskEven | tmpMaskOdd) {
			if (tmpMaskEven & 1) {
				dstIndices[result++] = 2 * i;
			}
			if (tmpMaskOdd & 1) {
				dstIndices[result++] = 2 * i + 1;
			}
			i++;
			tmpMaskEven >>= 1;
			tmpMaskOdd >>= 1;
			remainingBits--;
		}
		i += remainingBits;
	}
	while (result % 2) {
		dstIndices[result] = dstIndices[result - 1];
		result++;
	}
	return result;
}


}