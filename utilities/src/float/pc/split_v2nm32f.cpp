#include "nmtype.h"
#pragma code_section ".text_demo3d"
extern "C"{
	
void split_v2nm32f(v2nm32f* srcVec4, int step, float* dstX, float* dstY, int countVec){
	for(int i=0;i<countVec;i++){
		dstX[i] = srcVec4[step * i].v0;
		dstY[i] = srcVec4[step * i].v1;
	}
}

}