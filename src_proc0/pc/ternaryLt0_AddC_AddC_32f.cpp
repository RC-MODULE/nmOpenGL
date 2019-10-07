#include "nmtype.h"

extern "C"{
	
void ternaryLt0_AddC_AddC_32f(nm32f* srcFlags, nm32f* srcVec, float valueLeft, float valueRight, float* dstVec, int size){
	for(int i=0; i< size; i++){
		dstVec[i] = (srcFlags[i] < 0) ? srcVec[i] + valueLeft: srcVec[i] + valueRight;
	}
}

}
