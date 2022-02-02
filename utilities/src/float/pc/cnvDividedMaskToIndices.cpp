#include "nmtype.h"
#include "demo3d_common.h"
#include "stdio.h"

extern "C"{
int cnvDividedMaskToIndices(nm1* maskEven, nm1* maskOdd, int* indices, int size) {

#ifdef DEBUG
  if ((size < 0) || ((size & 1) != 0)) {
    printf ("%s:%d: Error: size must be non negative even number", __FILE__, __LINE__);
  }
#endif //DEBUG

	int result = 0;
	int i = 0;
	int i32 = 0;
	int* maskVecEven = ((int*)maskEven);
	int* maskVecOdd = ((int*)maskOdd);
	size /= 2;
	while (i < size) {
		unsigned int tmpMaskEven = maskVecEven[i32];
		unsigned int tmpMaskOdd = maskVecOdd[i32];
		i32++;
		unsigned int remainingBits = 32;
		while ((tmpMaskEven | tmpMaskOdd) && (i < size)) {
			if (tmpMaskEven & 1) {
				indices[result++] = 2 * i;
			}
			if (tmpMaskOdd & 1) {
				indices[result++] = 2 * i + 1;
			}
			i++;
			tmpMaskEven >>= 1;
			tmpMaskOdd >>= 1;
			remainingBits--;
		}
		i += remainingBits;
	}
	return result;
}
}
