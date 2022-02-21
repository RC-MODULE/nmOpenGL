#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "tests.h"
#include <float.h>
#include <math.h>

//#define DEBUG
int cmpNeC_f_size0_resultCorrect();
int cmpNeC_f_size1_resultCorrect();
int cmpNeC_f_size63_resultCorrect();
int cmpNeC_f_size64_resultCorrect();
int cmpNeC_f_size68_resultCorrect();
int cmpNeC_f_size69_resultCorrect();
int cmpNeC_f_evenNotEqual_maskCorrect();
int cmpNeC_f_oddNotEqual_maskCorrect();
int cmpNeC_f_comparisonTest_maskCorrect();
int cmpNeC_f_size69_dstMaskExtraWordsNotChanged();

extern "C" void cmpNeC_f(float* src, float C, nm1* dstMaskEven, nm1* dstMaskOdd, int size);

int main()
{

	  RUN_TEST(cmpNeC_f_size0_resultCorrect);
	  RUN_TEST(cmpNeC_f_size1_resultCorrect);
	  RUN_TEST(cmpNeC_f_size63_resultCorrect);
	  RUN_TEST(cmpNeC_f_size64_resultCorrect);
	  RUN_TEST(cmpNeC_f_size68_resultCorrect);
	  RUN_TEST(cmpNeC_f_size69_resultCorrect);
	  RUN_TEST(cmpNeC_f_evenNotEqual_maskCorrect);
	  RUN_TEST(cmpNeC_f_oddNotEqual_maskCorrect);
	  RUN_TEST(cmpNeC_f_comparisonTest_maskCorrect);
	  RUN_TEST(cmpNeC_f_size69_dstMaskExtraWordsNotChanged);

	return 0;
}

int cmpNeC_f_size0_resultCorrect(){

  float src[] = {1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1};
  float C = 1.0;
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0xffffffff};
  unsigned int refDstMaskOdd[] ={0xffffffff};  
  int size = 0;
  int resMaskCount = 1;

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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


int cmpNeC_f_size1_resultCorrect(){

  float src[] = {1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1};
  float C = 0.0;
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x1};
  unsigned int refDstMaskOdd[] ={0x0};  
  int size = 1;
  int resMaskCount = 1;

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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

int cmpNeC_f_size63_resultCorrect(){

  constexpr int size = 63;
  float src[size] = {0};
  float C = 0.0;
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0xFFFFFFFF};
  unsigned int refDstMaskOdd[] ={0x7FFFFFFF};  
  int resMaskCount = 1;

  for (int i=0; i<size; i++) {
    src[i] = 1.0;
  }

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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

int cmpNeC_f_size64_resultCorrect(){

  constexpr int size = 64;
  float src[size] = {0};
  float C = 0.0;
  unsigned int dstMaskEven[] = {0x00000000};
  unsigned int dstMaskOdd[] ={0x00000000};  
  unsigned int refDstMaskEven[] = {0xFFFFFFFF};
  unsigned int refDstMaskOdd[] ={0xFFFFFFFF};  
  int resMaskCount = 1;

  for (int i=0; i<size; i++) {
    src[i] = 1.0;
  }

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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

int cmpNeC_f_size68_resultCorrect(){

  constexpr int size = 68;
  float src[size] = {0};
  float C = 0.0;
  unsigned int dstMaskEven[] = {0xffffffff, 0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff, 0xffffffff};  
  unsigned int refDstMaskEven[] = {0x4,0x1};
  unsigned int refDstMaskOdd[] ={0x8,0x2};  
  int resMaskCount = 2;

  //Setup src values which correspond to reference dst masks
  for (int i=0; i<size; i++) {
    src[i] = 0.0;
  }

  src[4] = 1.0;
  src[7] = 1.0;
  src[64] = 1.0;
  src[67] = 1.0;

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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


int cmpNeC_f_size69_resultCorrect(){

  constexpr int size = 69;
  float src[size] = {0};
  float C = 0.0;
  unsigned int dstMaskEven[] = {0xffffffff, 0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff, 0xffffffff};  
  unsigned int refDstMaskEven[] = {0x4,0x5};
  unsigned int refDstMaskOdd[] ={0x8,0x2};  
  int resMaskCount = 2;

  //Setup src values which correspond to reference dst masks
  for (int i=0; i<size; i++) {
    src[i] = 0.0;
  }

  src[4] = 1.0;
  src[7] = 1.0;
  src[64] = 1.0;
  src[67] = 1.0;
  src[68] = 1.0;

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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


int cmpNeC_f_evenNotEqual_maskCorrect(){

  constexpr int size = 34;
  float src[size+2] = {0,0.5,1,0.5,0,0.5,1,0.5, 0,0.5,1,0.5,0,0.5,1,0.5, 0,0.5,1,0.5,0,0.5,1,0.5, 0,0.5,1,0.5,0,0.5,1,0.5, 0,0.5,0,0};
  float C = 0.5;
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x0001FFFF};
  unsigned int refDstMaskOdd[] ={0x00000000};  
  int resMaskCount = 1;

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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


int cmpNeC_f_oddNotEqual_maskCorrect(){

  constexpr int size = 34;
  float src[size+2] = {0.5,1,0.5,0,0.5,1,0.5,0, 0.5,1,0.5,0,0.5,1,0.5,0, 0.5,1,0.5,0,0.5,1,0.5,0, 0.5,1,0.5,0,0.5,1,0.5,0, 0.5,0,0,0};
  float C = 0.5;
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x00000000};
  unsigned int refDstMaskOdd[] ={0x0001FFFF};  
  int resMaskCount = 1;

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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


int cmpNeC_f_comparisonTest_maskCorrect(){

  constexpr int size = 32;
  float src[size] = {-FLT_MAX,-FLT_MIN,    -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,
                              -FLT_MIN,    -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,
                                           -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,-FLT_MIN,
                                                     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,-FLT_MIN,    -1.0
                    };
  float C = 0.0;
  unsigned int dstMaskEven[] = {0xffffffff};
  unsigned int dstMaskOdd[] ={0xffffffff};  
  unsigned int refDstMaskEven[] = {0x0000af5f};
  unsigned int refDstMaskOdd[] ={0x0000faf5};  
  int resMaskCount = 1;

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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


//Check that function does not change dstMasks more than it is needed
int cmpNeC_f_size69_dstMaskExtraWordsNotChanged(){

  constexpr int resMaskCount = 4;
  constexpr int srcSize = resMaskCount*32*2;//32 - size of one mask, 2 - two masks: even and odd 
  float src[srcSize] = {0};
  constexpr int size = 69;
  float C = 0.0;
  unsigned int dstMaskEven[resMaskCount]    = {0xffffffff, 0xffffffff, 0xAAAAAAAA, 0x55555555};
  unsigned int dstMaskOdd[resMaskCount]     = {0xffffffff, 0xffffffff, 0x55555555, 0xAAAAAAAA};  
  unsigned int refDstMaskEven[resMaskCount] = {0xffffffff,        0x7, 0xAAAAAAAA, 0x55555555};
  unsigned int refDstMaskOdd[resMaskCount]  = {0xffffffff,        0x3, 0x55555555, 0xAAAAAAAA};  

  //Set all alements of src1 and src2 so that comparison result will be true
  for (int i=0; i<srcSize; i++) {
    src[i] = 1.0;
  }

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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

  //Swap dstMask elements 2 and 3 and test one more time
  unsigned int tmp = 0;

  tmp = dstMaskEven[2];
  dstMaskEven[2] = dstMaskEven[3];
  dstMaskEven[3] = tmp;
  refDstMaskEven[2] = dstMaskEven[2];
  refDstMaskEven[3] = dstMaskEven[3];

  tmp = dstMaskOdd[2];
  dstMaskOdd[2] = dstMaskOdd[3];
  dstMaskOdd[3] = tmp;
  refDstMaskOdd[2] = dstMaskOdd[2];
  refDstMaskOdd[3] = dstMaskOdd[3];

  cmpNeC_f(src, C, (nm1*)dstMaskEven, (nm1*)dstMaskOdd, size);

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
