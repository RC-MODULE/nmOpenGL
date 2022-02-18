#include "nmtype.h"
#include "stdio.h"
#include "nmpp.h"

extern "C"{

void cmpNeV_f(float* src1, float* src2, nm1* dstMaskEven, nm1* dstMaskOdd, int size) {

  unsigned int i = 0;

  if (size <= 0) return;

  for (i=0; i<size; i++) {
    int compRes = src1[i] != src2[i] ? 1 : 0;
    if (i&1){
      nmppsPut_1(dstMaskOdd, i>>1, compRes);
    } else {
      nmppsPut_1(dstMaskEven, i>>1, compRes);
    }
  }

  unsigned int residue = (i<<26)>>26;
  if (residue == 0) return;
  residue = 64 - residue;
  unsigned int curIndex = i;
  for ( i=curIndex; i<curIndex + residue; i++) {
    if (i&1){
      nmppsPut_1(dstMaskOdd, i>>1, 0);
    } else {
      nmppsPut_1(dstMaskEven, i>>1, 0);
    }
  }

}

}
  
