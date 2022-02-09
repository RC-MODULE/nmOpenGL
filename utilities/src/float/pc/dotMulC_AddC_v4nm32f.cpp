#include "nmtype.h"

extern "C"{
	void dotMulC_AddC_v4nm32f(float* n_dot_VP, v4nm32f* srcDiffuse, v4nm32f* srcAmbient, v4nm32f* dst, int size){
		for(int i=0;i<size;i++){
			dst[i].vec[0] = n_dot_VP[2*i] * srcDiffuse[0].vec[0] + srcAmbient[0].vec[0];
			dst[i].vec[1] = n_dot_VP[2*i] * srcDiffuse[0].vec[1] + srcAmbient[0].vec[1];
			dst[i].vec[2] = n_dot_VP[2*i] * srcDiffuse[0].vec[2] + srcAmbient[0].vec[2];
			dst[i].vec[3] = n_dot_VP[2*i] * srcDiffuse[0].vec[3] + srcAmbient[0].vec[3];			
		}
	}
}