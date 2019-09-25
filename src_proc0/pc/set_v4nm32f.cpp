#include "nmtype.h"

extern "C"
void set_v4nm32f(v4nm32f* dstVec, v4nm32f* valueC, int size){
	for(int i=0;i<size;i++){
		dstVec[i].vec[0] = valueC[0].vec[0];
		dstVec[i].vec[1] = valueC[0].vec[1];
		dstVec[i].vec[2] = valueC[0].vec[2];
		dstVec[i].vec[3] = valueC[0].vec[3];
	}
}
