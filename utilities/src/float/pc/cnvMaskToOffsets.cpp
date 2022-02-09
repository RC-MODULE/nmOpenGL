#include "nmtype.h"
#include "demo3d_common.h"

extern "C" 
SECTION(".text_demo3d")
int cnvMaskToOffsets(nm1* mask, int* offsets, int size){
  int result = 0;
	int i = 0;
	int offset = 0;
	int i32 = 0;
	int* maskVec = ((int*)mask);

	while (i < size) {
		unsigned int tmpMask = maskVec[i32++];
		unsigned int remainingBits = 32;
		while (tmpMask && (i < size)) {
			if (tmpMask & 1) {
				offsets[result++] = offset;
				offset = 0;
			}
			i++;
			offset++;
			tmpMask >>= 1;
			remainingBits--;
		}
		i += remainingBits;
	}

	return result;
}
