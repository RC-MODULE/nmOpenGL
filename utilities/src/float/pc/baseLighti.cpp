#include "nmpp.h"


extern "C" {
	void baseLighti(v4nm32f* ambient, v2nm32f* n_dot_vp, v4nm32f* diffuse, v2nm32f* n_dot_h_in_srm, v4nm32f* specular, v4nm32f* dst, int count){
		for(int i = 0; i < count; i++){
			float f = (n_dot_vp[i].v0 != 0) ? 1: 0;
			for(int j = 0; j < 4; j++){
				dst[i].vec[j] = ambient->vec[j] + 
					n_dot_vp[i].v0 * diffuse->vec[j] + 
					f * n_dot_h_in_srm[i].v0 * specular->vec[j];
			}
		}
	}
}