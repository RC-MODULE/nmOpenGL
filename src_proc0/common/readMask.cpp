#include "nmtype.h"
extern "C"{
	
int readMask(nm1* mask, int* dstNumbers, int size){
	int result = 0;
	int i = 0;
	int i32 = 0;
	int* maskVec = ((int*)mask);
	int tmpMask;
	while(i < size){
		if(i % 32 == 0){
			tmpMask = maskVec[i32++];
			if(tmpMask == 0){
				i += 32;
				continue;
			}
		}
		if(tmpMask & 1){
			dstNumbers[result++] = i;
		}
		tmpMask>>=1;
		i++;
	}
	
	if (result & 1) {
		dstNumbers[result] = dstNumbers[result - 1];
		result++;
	}
	return result;
}

}