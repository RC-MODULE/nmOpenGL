#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "tests.h"
#include <float.h>
#include <math.h>

//#define DEBUG

int cmpLtCSelVC_f_size0_resultCorrect();
int cmpLtCSelVC_f_size1_resultCorrect();
int cmpLtCSelVC_f_size63_resultCorrect();
int cmpLtCSelVC_f_size64_resultCorrect();
int cmpLtCSelVC_f_size68_resultCorrect();
int cmpLtCSelVC_f_size69_resultCorrect();
int cmpLtCSelVC_f_oddLess_maskCorrect();
int cmpLtCSelVC_f_evenLess_maskCorrect();
int cmpLtCSelVC_f_comparisonTest_maskCorrect();
int cmpLtCSelVC_f_size69_dstMaskExtraWordsNotChanged();

#define ARRAY_SIZE 128

extern "C" void cmpLtCSelVC_f(float* src, float C, int* val1, int val2, int* dst, int size);

int main()
{

	  RUN_TEST(cmpLtCSelVC_f_size0_resultCorrect);
	  RUN_TEST(cmpLtCSelVC_f_size1_resultCorrect);
	  RUN_TEST(cmpLtCSelVC_f_size63_resultCorrect);
	  RUN_TEST(cmpLtCSelVC_f_size64_resultCorrect);
	  RUN_TEST(cmpLtCSelVC_f_size68_resultCorrect);
	  RUN_TEST(cmpLtCSelVC_f_size69_resultCorrect);
	  RUN_TEST(cmpLtCSelVC_f_oddLess_maskCorrect);
	  RUN_TEST(cmpLtCSelVC_f_evenLess_maskCorrect);
	  RUN_TEST(cmpLtCSelVC_f_comparisonTest_maskCorrect);
	  RUN_TEST(cmpLtCSelVC_f_size69_dstMaskExtraWordsNotChanged);

	return 0;
}

int cmpLtCSelVC_f_size0_resultCorrect(){
  float src[ARRAY_SIZE] = {0.0};
  float C = 1.0;
  int val1[ARRAY_SIZE] = {0};
  int val2 = 123;
  int dst[ARRAY_SIZE] = {0};
  int refDst[ARRAY_SIZE] = {0};
  int size = 0;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    src[i] = 0.0;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    val1[i] = i+1;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    refDst[i] = 0;
  }

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, refDst, ARRAY_SIZE);

  return 0;
}


int cmpLtCSelVC_f_size1_resultCorrect(){

  float src[ARRAY_SIZE] = {0.0};
  float C = 1.0;
  int val1[ARRAY_SIZE] = {0};
  int val2 = 123;
  int dst[ARRAY_SIZE] = {0};
  int refDst[ARRAY_SIZE] = {0};
  int size = 1;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    src[i] = 0.0;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    val1[i] = i+1;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    refDst[i] = 0;
  }
  refDst[0] = val1[0];

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, refDst, ARRAY_SIZE);

  return 0;
}

int cmpLtCSelVC_f_size63_resultCorrect(){

  float src[ARRAY_SIZE] = {0.0};
  float C = 1.0;
  int val1[ARRAY_SIZE] = {0};
  int val2 = 123;
  int dst[ARRAY_SIZE] = {0};
  int refDst[ARRAY_SIZE] = {0};
  int size = 63;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    src[i] = 0.0;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    val1[i] = i+1;
  }

  for (int i = 0; i < size; i++) {
    refDst[i] = val1[i];
  }

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, refDst, ARRAY_SIZE);

  return 0;
}

int cmpLtCSelVC_f_size64_resultCorrect(){

  float src[ARRAY_SIZE] = {0.0};
  float C = 1.0;
  int val1[ARRAY_SIZE] = {0};
  int val2 = 123;
  int dst[ARRAY_SIZE] = {0};
  int refDst[ARRAY_SIZE] = {0};
  int size = 64;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    src[i] = 0.0;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    val1[i] = i+1;
  }

  for (int i = 0; i < size; i++) {
    refDst[i] = val1[i];
  }

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, refDst, ARRAY_SIZE);

  return 0;
}

int cmpLtCSelVC_f_size68_resultCorrect(){

  float src[ARRAY_SIZE] = {0.0};
  float C = 1.0;
  int val1[ARRAY_SIZE] = {0};
  int val2 = 123;
  int dst[ARRAY_SIZE] = {0};
  int refDst[ARRAY_SIZE] = {0};
  int size = 68;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    src[i] = 0.0;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    val1[i] = i+1;
  }

  for (int i = 0; i < size; i++) {
    refDst[i] = val1[i];
  }

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, refDst, ARRAY_SIZE);

  return 0;
}


int cmpLtCSelVC_f_size69_resultCorrect(){

  float src[ARRAY_SIZE] = {0.0};
  float C = 1.0;
  int val1[ARRAY_SIZE] = {0};
  int val2 = 123;
  int dst[ARRAY_SIZE] = {0};
  int refDst[ARRAY_SIZE] = {0};
  int size = 69;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    src[i] = 0.0;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    val1[i] = i+1;
  }

  for (int i = 0; i < size; i++) {
    refDst[i] = val1[i];
  }

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, refDst, ARRAY_SIZE);

  return 0;
}


int cmpLtCSelVC_f_oddLess_maskCorrect(){

  constexpr int size = 34;
  float src[size] = {1,0,2,0,1,0,2,0, 1,0,2,0,1,0,2,0, 1,0,2,0,1,0,2,0, 1,0,2,0,1,0,2,0, 1,0};
  float C = 1.0;
  int val1[size] = {0};
  int val2 = 123;
  int dst[size] = {0};
  int refDst[size] = {0};

  for (int i = 0; i < size; i++) {
    val1[i] = i+1;
  }

  for (int i = 0; i < size; i++) {
	  if (i&1) refDst[i] = val1[i];
	  else refDst[i] = val2;
  }

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < size; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, refDst, size);

  return 0;
}


int cmpLtCSelVC_f_evenLess_maskCorrect(){

  constexpr int size = 34;
  float src[size] = {0,1,0,2,0,1,0,2, 0,1,0,2,0,1,0,2, 0,1,0,2,0,1,0,2, 0,1,0,2,0,1,0,2, 0,1};
  float C = 1.0;
  int val1[size] = {0};
  int val2 = 123;
  int dst[size] = {0};
  int refDst[size] = {0};

  for (int i = 0; i < size; i++) {
    val1[i] = i+1;
  }

  for (int i = 0; i < size; i++) {
	  if (i&1) refDst[i] = val2;
	  else refDst[i] = val1[i];
  }

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < size; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, refDst, size);

  return 0;
}


int cmpLtCSelVC_f_comparisonTest_maskCorrect(){

  constexpr int size = 32;
  float src[size] = {-FLT_MAX,-FLT_MIN,    -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,
                              -FLT_MIN,    -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,
                                           -1.0,     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,-FLT_MIN,
                                                     0.0,     1.0, FLT_MIN, FLT_MAX,     0.0,-FLT_MAX,-FLT_MIN,    -1.0
                    };
  int val1[size] = {1,2,3,4,5,6,7,8, 9,10,11,12,13,14,15,16, 17,18,19,20,21,22,23,24, 25,26,27,28,29,30,31,32};
  int val2 = 123;
  float C = 0.0;
  int dst[size] = {0};
  int refDst[size] = {1,2,3,123,123,123,123,123, 9,10,123,123,123,123,123,16, 17,123,123,123,123,123,23,24, 123,123,123,123,123,30,31,32};

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < size; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, refDst, size);

  return 0;
}


//Check that function does not change dstMasks more than it is needed
int cmpLtCSelVC_f_size69_dstMaskExtraWordsNotChanged(){

  float src[ARRAY_SIZE] = {0.0};
  float C = 1.0;
  int val1[ARRAY_SIZE] = {0};
  int val2 = 123;
  int dst[ARRAY_SIZE] = {0};
  int refDst[ARRAY_SIZE] = {0};
  int size = 69;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    src[i] = 0.0;
  }


  for (int i = 0; i < ARRAY_SIZE; i++) {
    val1[i] = i+1;
  }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    dst[i] = 65+i;
	refDst[i] = dst[i];
  }

  for (int i = 0; i < size; i++) {
    refDst[i] = val1[i];
  }

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);
  TEST_ARRAYS_EQUALI(dst, refDst, ARRAY_SIZE);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  int tmp = dst[size];
  dst[size] = refDst[size] = dst[size+1];
  dst[size+1] = refDst[size+1] = tmp;

  cmpLtCSelVC_f(src, C, val1, val2, dst, size);
  TEST_ARRAYS_EQUALI(dst, refDst, ARRAY_SIZE);

#ifdef DEBUG
  printf("\r\n");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("refDst= %d dst=%d\n", refDst[i],dst[i]);
  }
#endif //DEBUG

  return 0;
}
