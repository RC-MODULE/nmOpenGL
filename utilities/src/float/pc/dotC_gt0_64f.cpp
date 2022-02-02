#include "nmtype.h"

extern "C"{
	void dotC_gt0_64f(v4nm64f* normal, v4nm64f* C, double* dstValues, int size){
		for(int i=0;i<size;i++){
			dstValues[i] = normal[i].vec[0] * C->vec[0] + 
						   normal[i].vec[1] * C->vec[1] +
						   normal[i].vec[2] * C->vec[2] +
						   normal[i].vec[3] * C->vec[3];
			dstValues[i] = (dstValues[i] > 0) ? dstValues[i] : 0;
		}
	}
}

