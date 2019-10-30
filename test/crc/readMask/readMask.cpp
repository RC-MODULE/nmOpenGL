#include "nmtype.h"
#include "demo3d_common.h"

extern "C"{

SECTION(".text_demo3d")
int readMask(nm1* mask, int* dstIndices, int* treated, int size, int maxSize){
	int result = 0;
	int i = treated[0];
	int i32 = i / 32;
	int* maskVec = ((int*)mask);
	int tmpMask = maskVec[i32];
	i32 = (i % 32 == 0) ? i32 : ++i32;
	while(i < size && result != maxSize){
		if(i % 32 == 0){
			tmpMask = maskVec[i32++];
			if(tmpMask == 0){
				i += 32;
				continue;
			}
		}
		if(tmpMask & 1){
			dstIndices[result++] = i;
		}
		tmpMask>>=1;
		i++;
	}
	treated[0] = i;
	if (result & 1) {
		dstIndices[result] = dstIndices[result - 1];
		result++;
	}
	return result;
}

}