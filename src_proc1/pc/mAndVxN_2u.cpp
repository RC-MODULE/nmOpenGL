#include "nmtype.h"

extern "C"{

void mAndVxN_32u(nm32u** pSrc1, nm32u** pSrc2, nm32u** pDst, int* size, int count){
	for(int i=0;i<count;i++){
		int* src1= (int*)pSrc1[i];
		int* src2= (int*)pSrc2[i];
		int* dst= (int*)pDst[i];
		for(int s = 0; s < size[i]; s++){
			dst[s] = src1[s] & src2[s];
		}
	} 
}

}
