#include "nmpp.h"
#include "time.h"
#include <stdio.h>
#include "demo3d_common.h"
#include "tests.h"

#define		FLOAT_ARRAY_SRC_SIZE 3	
#define		FLOAT_ARRAY_DST_SIZE 9	

#define		INT_ARRAY_SRC_SIZE 3	
#define		INT_ARRAY_DST_SIZE 9	

#define		OBJECT_ARRAY_SRC_SIZE 3	
#define		OBJECT_ARRAY_DST_SIZE 64	

int deselectElementsByIndices_deselectFloats();
int deselectElementsByIndices_deselectInts();
int deselectElementsByIndices_deselectObjects();

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

extern "C" void deselectElementsByIndices(void* src, int* indices, void* dst, int sizeOfElementInt, int size);

int main()
{

  RUN_TEST(deselectElementsByIndices_deselectFloats);
  RUN_TEST(deselectElementsByIndices_deselectInts);
  RUN_TEST(deselectElementsByIndices_deselectObjects);

	return 0;
}


int deselectElementsByIndices_deselectFloats() {

  float src[FLOAT_ARRAY_SRC_SIZE] = {1.5,3.5,2.5};
  int indices[3]={1,3,2};
  int size = 3;
  float dst[FLOAT_ARRAY_DST_SIZE]   ={800.5, 0.0, 0.0, 0.0, 274.5, 721.5, 289.5, 777.5, 118.5};
  float dstRef[FLOAT_ARRAY_DST_SIZE]={800.5, 1.5, 2.5, 3.5, 274.5, 721.5, 289.5, 777.5, 118.5};

  deselectElementsByIndices(src, indices, dst, sizeof32(int), size);

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

int deselectElementsByIndices_deselectInts() {

  int src[INT_ARRAY_SRC_SIZE]={777, 888, 777};
  int indices[INT_ARRAY_SRC_SIZE + 1]={2,3,2,1};
  int size = 3;
  int dst[INT_ARRAY_DST_SIZE]   ={0xC0DE0001, 0xC0DE0002, 999, 999, 736, 884, 997, 457, 900};
  int dstRef[INT_ARRAY_DST_SIZE]={0xC0DE0001, 0xC0DE0002, 777, 888, 736, 884, 997, 457, 900};

  deselectElementsByIndices(src, indices, dst, sizeof32(int), size);

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


int deselectElementsByIndices_deselectObjects() {

  testClass src[OBJECT_ARRAY_SRC_SIZE];
  int indices[3] = {0,1,2};
  int size = 3;
  testClass dst[OBJECT_ARRAY_DST_SIZE];
  testClass dstRef[OBJECT_ARRAY_DST_SIZE];

  src[0] = testClass(1,'a',1.5,100);
  src[1] = testClass(2,'b',2.5,200);
  src[2] = testClass(3,'c',3.5,300);

  for (int i = 0; i < OBJECT_ARRAY_DST_SIZE; i++) {
    dst[i] = testClass(0,'z',0.5,777);
  }

  for (int i = 0; i < OBJECT_ARRAY_DST_SIZE; i++) {
    dstRef[i] = dst[i];
  }
  dstRef[indices[0]] = src[0];
  dstRef[indices[1]] = src[1];
  dstRef[indices[2]] = src[2];

  deselectElementsByIndices(src, indices, dst, sizeof32(testClass), size);

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

  TEST_ARRAYS_EQUALI(dst, dstRef, OBJECT_ARRAY_DST_SIZE);

  //test one more time
  src[0] = testClass(63,'f',63.5,6300);
  indices[0] = OBJECT_ARRAY_DST_SIZE-1; //copy to last element of dst
  size = 1;
  dstRef[indices[0]] = src[0];

  deselectElementsByIndices(src, indices, dst, sizeof32(testClass), size);

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

  TEST_ARRAYS_EQUALI(dst, dstRef, OBJECT_ARRAY_DST_SIZE);

  return 0;
}
