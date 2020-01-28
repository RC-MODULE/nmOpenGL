#include "nmtype.h"
extern "C"{
void doubleClamp_32f(nm32f* pSrcVec1, nm32f* pSrcVec2, float min, float max, nm32f* pDstVec1, nm32f* pDstVec2, int size){
	for(int i=0;i<size;i++){
		if(pSrcVec1[i]<min){
			pDstVec1[i] = min;
		} else if (pSrcVec1[i]>max) {
			pDstVec1[i] = max;	
		}
		else {
			pDstVec1[i] = pSrcVec1[i];	
		}
		
		if(pSrcVec2[i]<min){
			pDstVec2[i] = min;
		} else if (pSrcVec2[i]>max) {
			pDstVec2[i] = max;	
		}
		else {
			pDstVec2[i] = pSrcVec2[i];	
		}
	}
}
}
