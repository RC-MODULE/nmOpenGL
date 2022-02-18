#include "nmtype.h"
#include "section-hal.h"

extern "C" 
int cnvMaskToIndices(nm1* mask, int* indices, int size){
  int result = 0;
	int i = 0;
	int i32 = 0;
	int* maskVec = ((int*)mask);

	while (i < size) {
		unsigned int tmpMask = maskVec[i32++];
		unsigned int remainingBits = 32;
		while (tmpMask && (i < size)) {
			if (tmpMask & 1) {
				indices[result++] = i;
			}
			i++;
			tmpMask >>= 1;
			remainingBits--;
		}
		i += remainingBits;
	}

	return result;
}
