#include "nmtype.h"
#include "demo3d_common.h"
#include "stdio.h"

extern "C"{

SECTION(".text_demo3d")
int readMaskToLimitDst(nm1* mask, int* dstIndices, int* treated, int size, int maxSize){
	int result = 0;
	
	int i = treated[0];
	int i32 = i >> 5;
	int* maskVec = ((int*)mask);
	unsigned int one = 1 << (i % 32);
	int tmpMask = maskVec[i32++];
	while(i < size && result < maxSize){
		if(tmpMask & one){
			dstIndices[result++] = i;
		}
		i++;
		one <<= 1;
		if (one == 0) {
			while (  (tmpMask = maskVec[i32++]) == 0 && i < size) {
				i += 32;
			}
			one = 1;
		}
	}
	treated[0] = (i < size) ? i: size;
	if (result & 1) {
		dstIndices[result] = dstIndices[result - 1];
		result++;
	}
	return result;
}


}