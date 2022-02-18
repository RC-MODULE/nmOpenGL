#include "nmtype.h"
extern "C" {
void dotV_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int size){
	for(int i=0;i<size;i++){
		dstValues[i].v0 = 0;
		for(int j = 0; j < 4; j++){
			dstValues[i].v0 += srcVec1[i].vec[j] * srcVec2[i].vec[j];
		}
		dstValues[i].v1 = dstValues[i].v0;
	}
}
}