#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"
#include "tests.h"
#include <float.h>
#include <math.h>

//#define DEBUG
int cmpGteV_f_size0_resultCorrect();
int cmpGteV_f_size1_resultCorrect();
int cmpGteV_f_size63_resultCorrect();
int cmpGteV_f_size64_resultCorrect();
int cmpGteV_f_size68_resultCorrect();
int cmpGteV_f_size69_resultCorrect();
int cmpGteV_f_evenGreaterEqual_maskCorrect();
int cmpGteV_f_oddGreaterEqual_maskCorrect();
int cmpGteV_f_comparisonTest_maskCorrect();

extern "C" void cmpGteV_f(float* src1, float* src2, nm1* dstMaskEven, nm1* dstMaskOdd, int size);

int main()
{

	  RUN_TEST(cmpGteV_f_size0_resultCorrect);
	  RUN_TEST(cmpGteV_f_size1_resultCorrect);
	  RUN_TEST(cmpGteV_f_size63_resultCorrect);
	  RUN_TEST(cmpGteV_f_size64_resultCorrect);
	  RUN_TEST(cmpGteV_f_size68_resultCorrect);
	  RUN_TEST(cmpGteV_f_size69_resultCorrect);
	  RUN_TEST(cmpGteV_f_evenGreaterEqual_maskCorrect);
	  RUN_TEST(cmpGteV_f_oddGreaterEqual_maskCorrect);
	  RUN_TEST(cmpGteV_f_comparisonTest_maskCorrect);

	return 0;
}

int cmpGteV_f_size0_resultCorrect(){

  float src1[] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0};
  float src2[] = {1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0xffffffff};
  unsigned int refDstMaskOdd[] ={0xffffffff};  
  int size = 0;
  int resMaskCount = 1;

  cmpGteV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\r\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\r\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\r\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\r\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGteV_f_size1_resultCorrect(){

  float src1[] = {2,1,2,1,2,1,2,1, 2,1,2,1,2,1,2,1, 2,1,2,1,2,1,2,1, 2,1,2,1,2,1,2,1, 2,1,2,1};
  float src2[] = {1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x1};
  unsigned int refDstMaskOdd[] ={0x0};  
  int size = 1;
  int resMaskCount = 1;

  cmpGteV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\r\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\r\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\r\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\r\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}

int cmpGteV_f_size63_resultCorrect(){

  constexpr int size = 63;
  float src1[size] = {0};
  float src2[size] = {0};
  unsigned int dstMaskEven[1] = {0xffffffff};
  unsigned int dstMaskOdd[1] ={0xffffffff};  
  unsigned int refDstMaskEven[1] = {0xFFFFFFFF};
  unsigned int refDstMaskOdd[1] ={0x7FFFFFFF};  
  int resMaskCount = 1;

  for (int i=0; i<size; i=i+2) {
    src1[i] = 1.0;
  }

  for (int i=1; i<size; i=i+2) {
    src1[i] = 2.0;
  }

  for (int i=0; i<size; i=i+1) {
    src2[i] = 1.0;
  }

  cmpGteV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  printf ("%d\r\n", dstMaskEven);
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\r\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\r\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\r\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\r\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}

int cmpGteV_f_size64_resultCorrect(){

  constexpr int size = 64;
  float src1[size] = {0};
  float src2[size] = {0};
  unsigned int dstMaskEven[1] = {0x00000000};
  unsigned int dstMaskOdd[1] ={0x00000000};  
  unsigned int refDstMaskEven[1] = {0xFFFFFFFF};
  unsigned int refDstMaskOdd[1] ={0xFFFFFFFF};  
  int resMaskCount = 1;

  for (int i=0; i<size; i=i+2) {
    src1[i] = 1.0;
  }

  for (int i=1; i<size; i=i+2) {
    src1[i] = 2.0;
  }

  for (int i=0; i<size; i=i+1) {
    src2[i] = 1.0;
  }
  cmpGteV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\r\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\r\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\r\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\r\n", dstMaskOdd[i]);
  }
#endif //DEBUG
TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}

int cmpGteV_f_size68_resultCorrect(){

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
  src1[4] = 1.0;
  src1[7] = 2.0;
  src1[64] = 2.0;
  src1[67] = 1.0;

  for (int i=0; i<size; i++) {
    src2[i] = 1.0;
  }

  cmpGteV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\r\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\r\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\r\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\r\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGteV_f_size69_resultCorrect(){

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
  src1[4] = 1.0;
  src1[7] = 2.0;
  src1[64] = 2.0;
  src1[67] = 1.0;
  src1[68] = 1.0;

  for (int i=0; i<size; i++) {
    src2[i] = 1.0;
  }

  cmpGteV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\r\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\r\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\r\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\r\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGteV_f_evenGreaterEqual_maskCorrect(){

  constexpr int size = 34;
  float src1[size+2] = {2,3,1,3,2,3,1,3, 2,3,1,3,2,3,1,3, 2,3,1,3,2,3,1,3, 2,3,1,3,2,3,1,3, 2,3,1,2};
  float src2[size+2] = {1,4,1,4,1,4,1,4, 1,4,1,4,1,4,1,4, 1,4,1,4,1,4,1,4, 1,4,1,4,1,4,1,4, 1,4,1,1};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x0001FFFF};
  unsigned int refDstMaskOdd[] ={0x00000000};  
  int resMaskCount = 1;

  cmpGteV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\r\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\r\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\r\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\r\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGteV_f_oddGreaterEqual_maskCorrect(){

  constexpr int size = 34;
  float src1[size+2] = {3,1,3,2,3,1,3,2, 3,1,3,2,3,1,3,2, 3,1,3,2,3,1,3,2, 3,1,3,2,3,1,3,2, 3,2,1,2};
  float src2[size+2] = {4,1,4,1,4,1,4,1, 4,1,4,1,4,1,4,1, 4,1,4,1,4,1,4,1, 4,1,4,1,4,1,4,1, 4,1,1,1};
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x00000000};
  unsigned int refDstMaskOdd[] ={0x0001FFFF};  
  int resMaskCount = 1;

  cmpGteV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\r\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\r\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\r\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\r\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}


int cmpGteV_f_comparisonTest_maskCorrect(){

  constexpr int size = 32;
  float src1[size] = {-FLT_MAX,-FLT_MIN,    -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,
                               -FLT_MIN,    -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,
                                            -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,-FLT_MIN,
                                                      0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,-FLT_MIN,    -1.0
                     };
  float src2[size] = {0.0};

  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x000076ec};
  unsigned int refDstMaskOdd[] ={0x000376e};  
  int resMaskCount = 1;

  cmpGteV_f(src1, src2, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

#ifdef DEBUG
  for (int i=0; i<resMaskCount; i++) {
    printf("\r\n");
    printf("refDstMaskEven = %0x\r\n", refDstMaskEven[i]);
    printf("dstMaskEven = %0x\r\n", dstMaskEven[i]);
    printf("refDstMaskOdd = %0x\r\n", refDstMaskOdd[i]);
    printf("dstMaskOdd = %0x\r\n", dstMaskOdd[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dstMaskEven, refDstMaskEven, resMaskCount);
  TEST_ARRAYS_EQUALI(dstMaskOdd, refDstMaskOdd, resMaskCount);

  return 0;
}
