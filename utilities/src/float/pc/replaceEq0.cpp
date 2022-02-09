#include "nmtype.h"

extern "C"{
void replaceEq0_32f(float* srcVec, float* dstVec, int size, float nReplaceC){
	for(int i=0;i<size;i++){
		if(srcVec[i]==0){
			dstVec[i] = nReplaceC;
		}
		else{
			dstVec[i] = srcVec[i];
		}
	}
}
}