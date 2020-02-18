#include "nmtype.h"

extern "C"{
	void dotMulV_v4nm32f(v2nm32f* n_dot_VP, v4nm32f* mulVec, v4nm32f* dst, int size){
		for(int i=0;i<size;i++){
			dst[i].vec[0] = n_dot_VP[i].v0 * mulVec[i].vec[0];
			dst[i].vec[1] = n_dot_VP[i].v0 * mulVec[i].vec[1];
			dst[i].vec[2] = n_dot_VP[i].v0 * mulVec[i].vec[2];
			dst[i].vec[3] = n_dot_VP[i].v0 * mulVec[i].vec[3];			
		}
	}
}