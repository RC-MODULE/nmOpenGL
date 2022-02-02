#include "nmtype.h"
#pragma code_section ".text_demo3d"
extern "C"{
	
void split_v4nm32f(v4nm32f* srcVec4, int step, float* dstX, float* dstY, float* dstZ, float* dstW, int countVec){
	for(int i=0;i<countVec;i++){
		dstX[i] = srcVec4[step * i].vec[0];
		dstY[i] = srcVec4[step * i].vec[1];
		dstZ[i] = srcVec4[step * i].vec[2];
		dstW[i] = srcVec4[step * i].vec[3];
	}
}

}