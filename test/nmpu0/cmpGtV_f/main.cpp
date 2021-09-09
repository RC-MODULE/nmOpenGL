#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"
#include "tests.h"
#include <float.h>
#include <math.h>

//#define DEBUG
int cmpGtV_f_size0_resultCorrect();
int cmpGtV_f_size1_resultCorrect();
int cmpGtV_f_size63_resultCorrect();
int cmpGtV_f_size64_resultCorrect();
int cmpGtV_f_size68_resultCorrect();
int cmpGtV_f_size69_resultCorrect();
int cmpGtV_f_evenGreater_maskCorrect();
int cmpGtV_f_oddGreater_maskCorrect();
int cmpGtV_f_comparisonTest_maskCorrect();

extern "C" void cmpGtV_f(float* src1, float* src2, nm1* dstMaskEven, nm1* dstMaskOdd, int size);

int main()
{

	  RUN_TEST(cmpGtV_f_size0_resultCorrect);
	  RUN_TEST(cmpGtV_f_size1_resultCorrect);
	  RUN_TEST(cmpGtV_f_size63_resultCorrect);
	  RUN_TEST(cmpGtV_f_size64_resultCorrect);
	  RUN_TEST(cmpGtV_f_size68_resultCorrect);
	  RUN_TEST(cmpGtV_f_size69_resultCorrect);
	  RUN_TEST(cmpGtV_f_evenGreater_maskCorrect);
	  RUN_TEST(cmpGtV_f_oddGreater_maskCorrect);
	  RUN_TEST(cmpGtV_f_comparisonTest_maskCorrect);

	return 0;
}

int cmpGtV_f_size0_resultCorrect(){

  float src1[] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0};
  float src2[] = {1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0xffffffff};
  unsigned int refDstMaskOdd[] ={0xffffffff};  
  int size = 0;
  int resMaskCount = 1;

  cmpGtV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGtV_f_size1_resultCorrect(){

  float src1[] = {1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1};
  float src2[] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x1};
  unsigned int refDstMaskOdd[] ={0x0};  
  int size = 1;
  int resMaskCount = 1;

  cmpGtV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}

int cmpGtV_f_size63_resultCorrect(){

  constexpr int size = 63;
  float src1[size] = {0};
  float src2[size] = {0};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0xFFFFFFFF};
  unsigned int refDstMaskOdd[] ={0x7FFFFFFF};  
  int resMaskCount = 1;

  for (int i=0; i<size; i++) {
    src1[i] = 1.0;
  }

  for (int i=0; i<size; i++) {
    src2[i] = 0.0;
  }

  cmpGtV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}

int cmpGtV_f_size64_resultCorrect(){

  constexpr int size = 64;
  float src1[size] = {0};
  float src2[size] = {0};
  unsigned int dstMaskEven[] = {0x00000000};
  unsigned int dstMaskOdd[] ={0x00000000};  
  unsigned int refDstMaskEven[] = {0xFFFFFFFF};
  unsigned int refDstMaskOdd[] ={0xFFFFFFFF};  
  int resMaskCount = 1;

  for (int i=0; i<size; i++) {
    src1[i] = 1.0;
  }

  for (int i=0; i<size; i++) {
    src2[i] = 0.0;
  }

  cmpGtV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}

int cmpGtV_f_size68_resultCorrect(){

  constexpr int size = 68;
  float src1[size] = {0};
  float src2[size] = {0};
  unsigned int dstMaskEven[] = {0xffffffff, 0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff, 0xffffffff};  
  unsigned int refDstMaskEven[] = {0x4,0x1};
  unsigned int refDstMaskOdd[] ={0x8,0x2};  
  int resMaskCount = 2;

  //Setup src values which correspond to reference dst masks
  for (int i=0; i<size; i++) {
    src1[i] = 0.0;
  }

  for (int i=0; i<size; i++) {
    src2[i] = 0.0;
  }

  src1[4] = 1.0;
  src1[7] = 1.0;
  src1[64] = 1.0;
  src1[67] = 1.0;

  cmpGtV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGtV_f_size69_resultCorrect(){

  constexpr int size = 69;
  float src1[size] = {0};
  float src2[size] = {0};
  unsigned int dstMaskEven[] = {0xffffffff, 0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff, 0xffffffff};  
  unsigned int refDstMaskEven[] = {0x4,0x5};
  unsigned int refDstMaskOdd[] ={0x8,0x2};  
  int resMaskCount = 2;

  //Setup src values which correspond to reference dst masks
  for (int i=0; i<size; i++) {
    src1[i] = 0.0;
  }

  for (int i=0; i<size; i++) {
    src2[i] = 0.0;
  }

  src1[4] = 1.0;
  src1[7] = 1.0;
  src1[64] = 1.0;
  src1[67] = 1.0;
  src1[68] = 1.0;

  cmpGtV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGtV_f_evenGreater_maskCorrect(){

  constexpr int size = 34;
  float src1[size+2] = {2,3,2,4,2,3,2,4, 2,3,2,4,2,3,2,4, 2,3,2,4,2,3,2,4, 2,3,2,4,2,3,2,4, 2,3,2,2};
  float src2[size+2] = {1,3,1,4,1,3,1,4, 1,3,1,4,1,3,1,4, 1,3,1,4,1,3,1,4, 1,3,1,4,1,3,1,4, 1,3,1,1};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x0001FFFF};
  unsigned int refDstMaskOdd[] ={0x00000000};  
  int resMaskCount = 1;

  cmpGtV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGtV_f_oddGreater_maskCorrect(){

  constexpr int size = 34;
  float src1[size+2] = {3,2,4,2,3,2,4,2, 3,2,4,2,3,2,4,2, 3,2,4,2,3,2,4,2, 3,2,4,2,3,2,4,2, 3,2,2,2};
  float src2[size+2] = {3,1,4,1,3,1,4,1, 3,1,4,1,3,1,4,1, 3,1,4,1,3,1,4,1, 3,1,4,1,3,1,4,1, 3,1,1,1};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x00000000};
  unsigned int refDstMaskOdd[] ={0x0001FFFF};  
  int resMaskCount = 1;

  cmpGtV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGtV_f_comparisonTest_maskCorrect(){

  constexpr int size = 32;
  float src1[size] = {-FLT_MAX,-FLT_MIN,    -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,
                               -FLT_MIN,    -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,
                                            -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,-FLT_MIN,
                                                      0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,-FLT_MIN,    -1.0
                     };
  float src2[size] = {0};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x0000264C};
  unsigned int refDstMaskOdd[] ={0x0003264};  
  int resMaskCount = 1;

  for (int i=0; i<size; i++) {
    src2[i] = 0.0;
  }

  cmpGtV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}
