#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"
#include "tests.h"

#define		FLOAT_ARRAY_SRC_SIZE 9	
#define		FLOAT_ARRAY_DST_SIZE 3	

#define		INT_ARRAY_SRC_SIZE 9	
#define		INT_ARRAY_DST_SIZE 3	

#define		OBJECT_ARRAY_SRC_SIZE 64	
#define		OBJECT_ARRAY_DST_SIZE 3	

int selectElementsByIndices_selectFloats();
int selectElementsByIndices_selectInts();
int selectElementsByIndices_selectObjects();

class testClass {
public:
  int a;
  char b;
  float c;
  long d;
  testClass(int a = 1, char b = 'a', float c = 1.0f, long d = -1):a(a), b(b), c(c), d(d){}
  void print();
  friend bool operator!= (const testClass& c1, const testClass& c2);
 };

void testClass::print () {
 printf ("%d %c %f %ld\n", a, b, c, d);
}

bool operator!= (const testClass& c1, const testClass& c2){
  return (c1.a != c2.a ||
          c1.b != c2.b ||
          c1.c != c2.c ||
          c1.d != c2.d);
}

extern "C" void selectElementsByIndices(void* src, int* indices, void* dst, int sizeOfElementInt, int size);

int main()
{

  RUN_TEST(selectElementsByIndices_selectFloats);
  RUN_TEST(selectElementsByIndices_selectInts);
  RUN_TEST(selectElementsByIndices_selectObjects);

	return 0;
}


int selectElementsByIndices_selectFloats() {

  float src[FLOAT_ARRAY_SRC_SIZE]={0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5};
  int indices[3]={1,3,2};
  float dst[FLOAT_ARRAY_DST_SIZE];
  float dstRef[FLOAT_ARRAY_DST_SIZE]={1.5,3.5,2.5};
  int size = 3;

	for (int i = 0; i < FLOAT_ARRAY_DST_SIZE; i++) {
		dst[i] = 0;
	}

  selectElementsByIndices(src, indices, dst, sizeof32(int), size);

#ifdef DEBUG
  for(int i=0; i < FLOAT_ARRAY_DST_SIZE; i++){
    printf("dst[%d]=%d\n", i, dst[i]);
  }
  printf(".\n");

  for(int i=0; i < FLOAT_ARRAY_DST_SIZE; i++){
    printf("dstRef[%d]=%d\n", i, dstRef[i]);
  }
  printf(".\n");
#endif //DEBUG

  TEST_ARRAYS_EQUAL(dst, dstRef, size);

  return 0;
}

int selectElementsByIndices_selectInts() {

  int src[INT_ARRAY_SRC_SIZE]={0, 1, 2, 3, 4, 5, 6, 7, 8};
  int indices[4]={2,3,2,1};
  int dst[INT_ARRAY_DST_SIZE];
  int dstRef[INT_ARRAY_DST_SIZE]={2, 3, 2};
  int size = 3;

	for (int i = 0; i < INT_ARRAY_DST_SIZE; i++) {
		dst[i] = 0;
	}

  selectElementsByIndices(src, indices, dst, sizeof32(int), size);

#ifdef DEBUG
  for(int i=0; i < INT_ARRAY_DST_SIZE; i++){
    printf("dst[%d]=%d\n", i, dst[i]);
  }
  printf(".\n");

  for(int i=0; i < INT_ARRAY_DST_SIZE; i++){
    printf("dstRef[%d]=%d\n", i, dstRef[i]);
  }
  printf(".\n");
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, dstRef, size);

  return 0;

}


int selectElementsByIndices_selectObjects() {

  testClass src[OBJECT_ARRAY_SRC_SIZE];
  int indices[3] = {0,1,2};
  testClass dst[OBJECT_ARRAY_DST_SIZE];
  testClass dstRef[OBJECT_ARRAY_DST_SIZE];
  int size = 3;

  src[0] = testClass(1,'a',1.5,100);
  src[1] = testClass(2,'b',2.5,200);
  src[2] = testClass(3,'c',3.5,300);
  src[3] = testClass(4,'d',4.5,400);
  src[4] = testClass(5,'e',5.5,500);
  src[63] = testClass(63,'f',63.5,6300);

  dstRef[0] = src[indices[0]];
  dstRef[1] = src[indices[1]];
  dstRef[2] = src[indices[2]];

  selectElementsByIndices(src, indices, dst, sizeof32(testClass), size);

#ifdef DEBUG
  for(int i=0; i < OBJECT_ARRAY_DST_SIZE; i++){
    printf("dst[%d]=", i);
    dst[i].print();
  }
  printf(".\n");

  for(int i=0; i < OBJECT_ARRAY_DST_SIZE; i++){
    printf("dstRef[%d]=", i);
    dstRef[i].print();
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, dstRef, size);

  indices[0] = 63;
  dstRef[0] = src[indices[0]];
  size = 1;

  selectElementsByIndices(src, indices, dst, sizeof32(testClass), size);

#ifdef DEBUG
  for(int i=0; i < OBJECT_ARRAY_DST_SIZE; i++){
    printf("dst[%d]=", i);
    dst[i].print();
  }
  printf(".\n");

  for(int i=0; i < OBJECT_ARRAY_DST_SIZE; i++){
    printf("dstRef[%d]=", i);
    dstRef[i].print();
  }
#endif //DEBUG

  TEST_ARRAYS_EQUALI(dst, dstRef, size);

  return 0;
}
