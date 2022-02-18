#include "nmtype.h"
#include "stdio.h"
#include "nmpp.h"

extern "C"{

void cmpLtCSelVC_f(float* src, float C, int* val1, int val2, int* dst, int size) {

  unsigned int i = 0;

  if (size <= 0) return;

  for (i=0; i<size; i++) {
    if (src[i] < C)
        dst[i] = val1[i];
    else
        dst[i] = val2;
  }

}
}
  
