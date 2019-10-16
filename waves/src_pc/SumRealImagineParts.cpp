#include <cstring>
#include "nmtype.h"

extern "C" {
	void SumReImParts(const nm32fcr* complex_nums, nm32f* sum_re_im, int size) {
	  for(int i = 0; i < size; i++) {
		sum_re_im[i] = complex_nums[i].re + complex_nums[i].im;
	  }
	}
	void halCopy_32s(void* src, void* dst, int size) {
	/*for (int i = 0; i < size; i++) {
		((int*)dst)[i] = ((int*)src)[i];
	}*/
	  memcpy(dst, src, size << 2);
    }
}

