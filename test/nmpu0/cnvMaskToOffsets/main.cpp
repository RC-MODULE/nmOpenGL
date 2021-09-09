#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"
#include "tests.h"

#define		TEST_MASK_SIZE 		17	

int cnvMaskToOffsets_allOnes_returns17();
int cnvMaskToOffsets_allZeroes_returns0();
int cnvMaskToOffsets_randomOnes_returns9();
int cnvMaskToOffsets_size0_resultCorrect();
int cnvMaskToOffsets_size1_resultCorrect();
int cnvMaskToOffsets_size32_resultCorrect();
int cnvMaskToOffsets_size33_resultCorrect();

extern "C" int cnvMaskToOffsets(nm1* mask, int* offsets, int size);
nm32s arrayToInt (unsigned int* array, int count);

int main()
{
  RUN_TEST(cnvMaskToOffsets_allOnes_returns17);
  RUN_TEST(cnvMaskToOffsets_allZeroes_returns0);
  RUN_TEST(cnvMaskToOffsets_randomOnes_returns9);
  RUN_TEST(cnvMaskToOffsets_size0_resultCorrect);
  RUN_TEST(cnvMaskToOffsets_size1_resultCorrect);
  RUN_TEST(cnvMaskToOffsets_size32_resultCorrect);
  RUN_TEST(cnvMaskToOffsets_size33_resultCorrect);

	return 0;
}

nm32s arrayToInt (unsigned int* array, int count) 
{
  nm32s res = 0;
  for (int i=0; i<count; i++)
  {
    res = res | (array[i] << i);
  }
  return res;
}

int cnvMaskToOffsets_allOnes_returns17(){

  unsigned int testMaskArray[TEST_MASK_SIZE] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1};
  constexpr int size = TEST_MASK_SIZE;
  int dstVec[size];
  int refDstVec[size]               = {0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1};
  int refResult = 17; 

  nm32s testMask = arrayToInt (testMaskArray, TEST_MASK_SIZE);
  int result = cnvMaskToOffsets((nm1*)(&testMask), dstVec, size);

#ifdef DEBUG
  printf("testMask = %0x\n", testMask);
  printf("result = %d\n", result);
  printf("dstVec: ");
  for (int i = 0; i < size; i++){
    printf("%3d ", dstVec[i]);
  }
  printf("\r\n");

  printf("refVec: ");
  for (int i = 0; i < size; i++){
    printf("%3d ", refDstVec[i]);
  }
  printf("\r\n");
#endif //DEBUG

  TEST_ASSERT(result == refResult);
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, refResult);

  return 0;
}

int cnvMaskToOffsets_allZeroes_returns0(){

  unsigned int testMaskArray[TEST_MASK_SIZE] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0};
  constexpr int size = TEST_MASK_SIZE;
  int dstVec[size];
  int refDstVec[size];
  int refResult  = 0; 

  for (int i = 0; i < size; i++) {
    dstVec[i] = i;
    refDstVec[i] = dstVec[i];
  }

  nm32s testMask = arrayToInt (testMaskArray, TEST_MASK_SIZE);
  int result = cnvMaskToOffsets((nm1*)(&testMask), dstVec, size);

#ifdef DEBUG
  printf("testMask = %0x\n", testMask);
  printf("result = %d\n", result);
  printf("dstVec: ");
  for (int i = 0; i < size; i++){
    printf("%3d ", dstVec[i]);
  }
  printf("\r\n");

  printf("refVec: ");
  for (int i = 0; i < size; i++){
    printf("%3d ", refDstVec[i]);
  }
  printf("\r\n");
#endif //DEBUG

  TEST_ASSERT(result == refResult);
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, TEST_MASK_SIZE);//Also check that dstVec is not changed

  return 0;
}

int cnvMaskToOffsets_randomOnes_returns9(){

  unsigned int testMaskArray[TEST_MASK_SIZE] = {0,0,1,1, 0,1,0,1, 0,1,0,1, 1,1,0,1, 0};
  constexpr int size = TEST_MASK_SIZE;
  int dstVec[size];
  int refDstVec[size]               = {    2,1,   2,  2,   2,  2, 1,1,  2};
  int refResult = 9; 

  nm32s testMask = arrayToInt (testMaskArray, TEST_MASK_SIZE);
  int result = cnvMaskToOffsets((nm1*)(&testMask), dstVec, size);

#ifdef DEBUG
  printf("testMask = %0x\n", testMask);
  printf("result = %d\n", result);
  printf("dstVec: ");
  for (int i = 0; i < size; i++){
    printf("%3d ", dstVec[i]);
  }
  printf("\r\n");

  printf("refVec: ");
  for (int i = 0; i < size; i++){
    printf("%3d ", refDstVec[i]);
  }
  printf("\r\n");
#endif //DEBUG

  TEST_ASSERT(result == refResult);
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, refResult);

  return 0;
}

int cnvMaskToOffsets_size0_resultCorrect(){

  constexpr int maxResult = 32;
  int dstVec[maxResult] = {0};
  unsigned int testMask = 0xffffffff;
  int size = 0;
  int refDstVec[maxResult] = {0};
  int refResult = 0; 

  for (int i = 0; i < maxResult; i++) {
    dstVec[i] = i;
    refDstVec[i] = dstVec[i];
  }

  int result = cnvMaskToOffsets((nm1*)(&testMask), dstVec, size);

#ifdef DEBUG
  printf("testMask = %0x\n", testMask);
  printf("result = %d\n", result);
  printf("dstVec: ");
  for (int i = 0; i < maxResult; i++){
    printf("%3d ", dstVec[i]);
  }
  printf("\r\n");

  printf("refVec: ");
  for (int i = 0; i < maxResult; i++){
    printf("%3d ", refDstVec[i]);
  }
  printf("\r\n");
#endif //DEBUG

  TEST_ASSERT(result == refResult);
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);

  return 0;
}

int cnvMaskToOffsets_size1_resultCorrect(){

  constexpr int maxResult = 32;
  int dstVec[maxResult] = {777,777,777}; //some not null values at the beginning
  unsigned int testMask = 0xffffffff;
  int size = 1;
  int refResult = 1; 
  int refDstVec[maxResult] = {0,777,777};

  int result = cnvMaskToOffsets((nm1*)(&testMask), dstVec, size);

#ifdef DEBUG
  printf("testMask = %0x\n", testMask);
  printf("result = %d\n", result);
  printf("dstVec: ");
  for (int i = 0; i < maxResult; i++){
    printf("%3d ", dstVec[i]);
  }
  printf("\r\n");

  printf("refVec: ");
  for (int i = 0; i < maxResult; i++){
    printf("%3d ", refDstVec[i]);
  }
  printf("\r\n");
#endif //DEBUG

  TEST_ASSERT(result == refResult);
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);

  return 0;
}

int cnvMaskToOffsets_size32_resultCorrect(){

  constexpr int maxResult = 64;
  int dstVec[maxResult] = {0}; 
  unsigned int testMask[2] = {0xc0000000, 0xffffffff};
  int size = 32;
  int refResult = 2; 
  int refDstVec[maxResult] = {30,1};

  int result = cnvMaskToOffsets((nm1*)(&testMask), dstVec, size);

#ifdef DEBUG
  for (int i = 0; i < 2; i++){
    printf("testMask[%d] = %0x\n", i, testMask[i]);
  }
  printf("size= %d\n", size);
  printf("result = %d\n", result);
  printf("dstVec: ");
  for (int i = 0; i < maxResult; i++){
    printf("%3d ", dstVec[i]);
  }
  printf("\r\n");

  printf("refVec: ");
  for (int i = 0; i < maxResult; i++){
    printf("%3d ", refDstVec[i]);
  }
  printf("\r\n");
#endif //DEBUG

  TEST_ASSERT(result == refResult);
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);

  return 0;
}

int cnvMaskToOffsets_size33_resultCorrect(){

  constexpr int maxResult = 64;
  int dstVec[maxResult] = {0}; 
  unsigned int testMask[2] = {0xc0000000, 0xffffffff};
  int size = 33;
  int refResult = 3; 
  int refDstVec[maxResult] = {30,1,1};

  int result = cnvMaskToOffsets((nm1*)(&testMask), dstVec, size);

#ifdef DEBUG
  for (int i = 0; i < 2; i++){
    printf("testMask[%d] = %0x\n", i, testMask[i]);
  }
  printf("size= %d\n", size);
  printf("result = %d\n", result);
  printf("dstVec: ");
  for (int i = 0; i < maxResult; i++){
    printf("%3d ", dstVec[i]);
  }
  printf("\r\n");

  printf("refVec: ");
  for (int i = 0; i < maxResult; i++){
    printf("%3d ", refDstVec[i]);
  }
  printf("\r\n");
#endif //DEBUG

  TEST_ASSERT(result == refResult);
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);

  return 0;
}
