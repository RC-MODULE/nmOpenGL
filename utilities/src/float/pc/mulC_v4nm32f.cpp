#include "nmtype.h"
extern "C"{
void mulC_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int size){
	int i;
	for(i=0;i<size;i++){
		pDst[i].vec[0] = pSrcV[i].vec[0] * pSrcC[0].vec[0];
		pDst[i].vec[1] = pSrcV[i].vec[1] * pSrcC[0].vec[1];
		pDst[i].vec[2] = pSrcV[i].vec[2] * pSrcC[0].vec[2];
		pDst[i].vec[3] = pSrcV[i].vec[3] * pSrcC[0].vec[3];
	}
}
}

