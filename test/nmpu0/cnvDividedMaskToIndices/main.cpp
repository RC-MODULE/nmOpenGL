#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"
#include "tests.h"

#define TEST_MASK_SIZE 17

int cnvDividedMaskToIndices_allOnes_returns34();
int cnvDividedMaskToIndices_allZeroes_returns0();
int cnvDividedMaskToIndices_randomOnes_returns18();
int cnvDividedMaskToIndices_longDistanceBetweenOnes_returns2();
int cnvDividedMaskToIndices_size0_resultCorrect();
int cnvDividedMaskToIndices_size2_resultCorrect();
int cnvDividedMaskToIndices_size64_resultCorrect();
int cnvDividedMaskToIndices_size66_resultCorrect();
int cnvDividedMaskToIndices_size66_dstVecExtraElementsNotChanged();

extern "C" int cnvDividedMaskToIndices(nm1* maskEven, nm1* maskOdd, int* offsets, int size);
nm32s arrayToInt (unsigned int* array, int count);

int main()
{
  RUN_TEST(cnvDividedMaskToIndices_allOnes_returns34);
  RUN_TEST(cnvDividedMaskToIndices_allZeroes_returns0);
  RUN_TEST(cnvDividedMaskToIndices_randomOnes_returns18);
  RUN_TEST(cnvDividedMaskToIndices_longDistanceBetweenOnes_returns2);
  RUN_TEST(cnvDividedMaskToIndices_size0_resultCorrect);
  RUN_TEST(cnvDividedMaskToIndices_size2_resultCorrect);
  RUN_TEST(cnvDividedMaskToIndices_size64_resultCorrect);
  RUN_TEST(cnvDividedMaskToIndices_size66_resultCorrect);
  RUN_TEST(cnvDividedMaskToIndices_size66_dstVecExtraElementsNotChanged);
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

int cnvDividedMaskToIndices_allOnes_returns34(){

  unsigned int testMaskEvenArray[TEST_MASK_SIZE] = {1,  1,  1,  1,   1,  1,  1,  1,     1,  1,  1,  1,     1,  1,  1,  1,    1};
  unsigned int testMaskOddArray[TEST_MASK_SIZE]  =   {1,  1,  1,  1,   1,  1,  1,  1,     1,  1,  1,  1,     1,  1,  1,  1,    1};
  constexpr int size = TEST_MASK_SIZE*2;
  int dstVec[size] = {0};
  int refResult = 34; 
  int refDstVec[size] ={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33};

  nm32s testMaskEven = arrayToInt (testMaskEvenArray, TEST_MASK_SIZE);
  nm32s testMaskOdd = arrayToInt (testMaskOddArray, TEST_MASK_SIZE);
  int result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG
  printf("testMaskEven = %0x\n", testMaskEven);
  printf("testMaskOdd = %0x\n", testMaskOdd);
  printf("size= %d\n", size);
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

int cnvDividedMaskToIndices_allZeroes_returns0(){

  unsigned int testMaskEvenArray[TEST_MASK_SIZE] = {0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0};
  unsigned int testMaskOddArray[TEST_MASK_SIZE]  = {0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0};
  constexpr int size = TEST_MASK_SIZE*2;
  int dstVec[size];
  int refResult = 0; 
  int refDstVec[size];

  for (int i = 0; i < size; i++) {
    dstVec[i] = i;
    refDstVec[i] = dstVec[i];
  }

  nm32s testMaskEven = arrayToInt (testMaskEvenArray, TEST_MASK_SIZE);
  nm32s testMaskOdd = arrayToInt (testMaskOddArray, TEST_MASK_SIZE);
  int result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG
  printf("testMaskEven = %0x\n", testMaskEven);
  printf("testMaskOdd = %0x\n", testMaskOdd);
  printf("size= %d\n", size);
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
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, size);//Also check that dstVec is not changed

  return 0;
}


int cnvDividedMaskToIndices_randomOnes_returns18(){

  unsigned int testMaskEvenArray[TEST_MASK_SIZE] = {0,  0,  1,  1,   0,  1,  0,  1,   0,  1,  0,  1,   1,  1,  0,  1,   0};
  unsigned int testMaskOddArray[TEST_MASK_SIZE]  =   {0,  0,  1,  1,   0,  1,  0,  1,   0,  1,  0,  1,   1,  1,  0,  1,   0};
  constexpr int size = TEST_MASK_SIZE*2;
  int dstVec[size];
  int refResult = 18; 
  int refDstVec[size] ={4,5,6,7,10,11,14,15,18,19,22,23,24,25,26,27,30,31};

  nm32s testMaskEven = arrayToInt (testMaskEvenArray, TEST_MASK_SIZE);
  nm32s testMaskOdd = arrayToInt (testMaskOddArray, TEST_MASK_SIZE);
  int result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG
  printf("testMaskEven = %0x\n", testMaskEven);
  printf("testMaskOdd = %0x\n", testMaskOdd);
  printf("size= %d\n", size);
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

int cnvDividedMaskToIndices_longDistanceBetweenOnes_returns2(){

  unsigned int testMaskEven[2] = {0x00000002, 0x00000000}; 
  unsigned int testMaskOdd[2] =  {0x00000000, 0x80000000}; 
  constexpr int size = 128;
  int dstVec[size];
  int refResult = 2; 
  int refDstVec[size] ={2, 127};

  int result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG
  for (int i = 0; i < 2; i++){
    printf("testMaskEven[%d] = %0x\n", i, testMaskEven[i]);
  }
  for (int i = 0; i < 2; i++){
    printf("testMaskOdd[%d] = %0x\n", i, testMaskOdd[i]);
  }
  printf("size= %d\n", size);
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


int cnvDividedMaskToIndices_size0_resultCorrect(){

  constexpr int maxResult = 64;
  unsigned int testMaskEven = 0xffffffff; 
  unsigned int testMaskOdd =  0xffffffff; 
  int dstVec[maxResult];
  int size = 0;
  int refResult = 0; 
  int refDstVec[maxResult];

  for (int i = 0; i < maxResult; i++) {
    dstVec[i] = i;
    refDstVec[i] = dstVec[i];
  }
  int result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG
  printf("testMaskEven = %0x\n", testMaskEven);
  printf("testMaskOdd = %0x\n", testMaskOdd);
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
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);//check that dstVec is not changed

  return 0;
}

int cnvDividedMaskToIndices_size2_resultCorrect(){

  constexpr int maxResult = 64;
  unsigned int testMaskEven = 0xffffffff; 
  unsigned int testMaskOdd =  0xffffffff; 
  int dstVec[maxResult] = {0};
  int size = 2;
  int refResult = 2; 
  int refDstVec[maxResult] = {0,1};

  int result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG
  printf("testMaskEven = %0x\n", testMaskEven);
  printf("testMaskOdd = %0x\n", testMaskOdd);
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
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);//count > refResult, test that dstVec is changed only in expected values

  return 0;
}

int cnvDividedMaskToIndices_size64_resultCorrect(){

  constexpr int maxResult = 128;
  unsigned int testMaskEven[] = {0x80000000, 0xffffffff}; 
  unsigned int testMaskOdd[] =  {0x80000000, 0xffffffff }; 
  int dstVec[maxResult] = {0};
  int size = 64;
  int refResult = 2; 
  int refDstVec[maxResult] = {62, 63};

  int result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG
  for (int i = 0; i < 2; i++){
    printf("testMaskEven[%d] = %0x\n", i, testMaskEven[i]);
  }
  for (int i = 0; i < 2; i++){
    printf("testMaskOdd[%d] = %0x\n", i, testMaskOdd[i]);
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
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);//count > refResult, test that dstVec is changed only in expected values

  return 0;
}


int cnvDividedMaskToIndices_size66_resultCorrect(){

  constexpr int maxResult = 128;
  unsigned int testMaskEven[] = {0x80000000, 0xffffffff}; 
  unsigned int testMaskOdd[] =  {0x80000000, 0xffffffff}; 
  int dstVec[maxResult] = {0};
  int size = 66;
  int refResult = 4; 
  int refDstVec[maxResult] = {62, 63, 64, 65};

  int result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG

  for (int i = 0; i < 2; i++){
    printf("testMaskEven[%d] = %0x\n", i, testMaskEven[i]);
  }
  for (int i = 0; i < 2; i++){
    printf("testMaskOdd[%d] = %0x\n", i, testMaskOdd[i]);
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
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);//count > refResult, test that dstVec is changed only in expected values

  return 0;
}



int cnvDividedMaskToIndices_size66_dstVecExtraElementsNotChanged(){

  constexpr int maxResult = 128;
  unsigned int testMaskEven[] = {0x80000000, 0xffffffff}; 
  unsigned int testMaskOdd[] =  {0x80000000, 0xffffffff}; 
  int dstVec[maxResult] = {0};
  int size = 66;
  int refResult = 4; 
  int refDstVec[maxResult] = {62, 63, 64, 65};

  dstVec[4] = refDstVec[4] = 777;
  dstVec[5] = refDstVec[5] = 888;

  int result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG

  for (int i = 0; i < 2; i++){
    printf("testMaskEven[%d] = %0x\n", i, testMaskEven[i]);
  }
  for (int i = 0; i < 2; i++){
    printf("testMaskOdd[%d] = %0x\n", i, testMaskOdd[i]);
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
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);//count > refResult, test that dstVec is changed only in expected values

  //swap extra elements and test one more time

  unsigned int tmp = 0;
  tmp = refDstVec[4];
  dstVec[4] = refDstVec[4] = refDstVec[6];
  dstVec[5] = refDstVec[5] = tmp;

  result = cnvDividedMaskToIndices((nm1*)(&testMaskEven), (nm1*)(&testMaskOdd), dstVec, size);

#ifdef DEBUG

  for (int i = 0; i < 2; i++){
    printf("testMaskEven[%d] = %0x\n", i, testMaskEven[i]);
  }
  for (int i = 0; i < 2; i++){
    printf("testMaskOdd[%d] = %0x\n", i, testMaskOdd[i]);
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
  TEST_ARRAYS_EQUALI(dstVec, refDstVec, maxResult);//count > refResult, test that dstVec is changed only in expected values

  return 0;
}
