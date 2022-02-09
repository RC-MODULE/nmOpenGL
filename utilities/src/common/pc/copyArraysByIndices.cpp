#include "nmtype.h"
#include "demo3d_common.h"

extern "C" 
SECTION(".text_demo3d")
void copyArraysByIndices(void** srcPointers, int* indices, void** dstPointers, int nArrays, int size){
	int** src = (int**)srcPointers;
	int** dst = (int**)dstPointers;
	for(int i=0; i < size; i++){
		for(int k = 0; k < nArrays; k++){
			dst[k][i] = src[k][indices[i]];
		}
	}
}