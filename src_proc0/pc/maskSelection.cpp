#include "nmtype.h"
extern "C"{
	
int maskSelection(void** src, nm1** mask, void* dst, int* size, int maxSize){
	int* currentSrc = (int*)*src;
	int* currentDst = (int*)dst;
	int currentSize = *size;
	int result = 0;
	int i= 0;
	int i32 = 0;
	int* maskVec = ((int**)mask)[0];
	int tmpMask;
	while(currentSize > 0 && result !=maxSize){
		if(i % 32 == 0){
			tmpMask = maskVec[i32];
			i32++;
		}
		if(tmpMask & 1){
			currentDst[result] = currentSrc[i];
			result++;
		}
		tmpMask>>=1;
		i++;
		currentSize--;
	}
	i32 = (i % 32 == 0) ? i32 : i32 - 1;
	src[0] = &currentSrc[i32 * 32];	
	mask[0] = (nm1*)&maskVec[i32];
	size[0] = currentSize;
	return result;
}

}