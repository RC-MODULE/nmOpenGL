#include "nmtype.h"

extern "C"{
	void dotC_gt0_32f(v4nm32f* normal, v4nm32f* C, nm32f* dstValues, int size){
		for(int i=0;i<size;i++){
			dstValues[2 * i] = normal[i].vec[0] * C->vec[0] + 
						   normal[i].vec[1] * C->vec[1] +
						   normal[i].vec[2] * C->vec[2] +
						   normal[i].vec[3] * C->vec[3];
			dstValues[2 * i] = (dstValues[2 * i] > 0) ? dstValues[2 * i] : 0;
			dstValues[2 * i + 1] = dstValues[2 * i];
		}
	}
}

