#include "nmtype.h"

extern "C"{
	void dotV_gt0_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int size){
		for(int i=0;i<size;i++){
			dstValues[i].v0 = srcVec1[i].vec[0] * srcVec2[i].vec[0] + 
							   srcVec1[i].vec[1] * srcVec2[i].vec[1] +
							   srcVec1[i].vec[2] * srcVec2[i].vec[2] +
							   srcVec1[i].vec[3] * srcVec2[i].vec[3];
			dstValues[i].v0 = (dstValues[i].v0 > 0) ? dstValues[i].v0 : 0;
			dstValues[i].v1 = dstValues[i].v0;
		} 
	}
}

