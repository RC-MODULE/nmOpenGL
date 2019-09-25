#include "nmtype.h"

extern "C"{
void copyPacket_32s(nm32s** pSrc, nm32s** pDst, int* size, int count){
	for(int i=0;i<count;i++){
		nm32s* src = pSrc[i];
		nm32s* dst = pDst[i];
		for(int k=0;k<size[i];k++){
			dst[k] = src[k];
		}
	}
}
}
