#include "nmtype.h"

extern "C" 
void copyArraysByIndices(void** srcPointers, int* indices, void** dstPointers, int nArrays, int size){
	int** src = (int**)srcPointers;
	int** dst = (int**)dstPointers;
	for(int i=0; i < size; i++){
		for(int k = 0; k < nArrays; k++){
			dst[k][i] = src[k][indices[i]];
		}
	}
}