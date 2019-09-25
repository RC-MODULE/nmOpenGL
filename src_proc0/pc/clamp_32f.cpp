#include "nmtype.h"
extern "C"{
void clamp_32f(nm32f* pSrcVec, float min, float max, nm32f* pDstVec, int size){
	for(int i=0;i<size;i++){
		if(pSrcVec[i]<min){
			pDstVec[i] = min;
		} else if (pSrcVec[i]>max) {
			pDstVec[i] = max;	
		}
		else {
			pDstVec[i] = pSrcVec[i];	
		}
	}
}
}
