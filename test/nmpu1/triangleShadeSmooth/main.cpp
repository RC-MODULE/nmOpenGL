#include <cstdio>
#include <cstdlib>
#include "nmtype.h"
#include "nmgldef.h"
#include "nmgltex_common.h"
#include "nmgltex_nm1.h"
#include "demo3d_nm1.h"
#include "nmplv/vSupport.h"
#include "malloc32.h"
#include "time.h"
#include "compareImages.h"
#include "references.h"

#ifndef __NM__
#include "bmp.h"
#include "image.h"
#endif

#ifdef __GNUC__
#define SECTION(sec) __attribute__((section(sec)))
#else
#define SECTION(sec)
#endif // __GNUC__

using namespace tex_nm1;

#define TRIANGLE_AMOUNT 5 

#ifndef __NM__
//массив изображений после закрашивания 
image_t result_images [1]; 


/**
Функция rawToImage выполняет преобразование массива цветов пикселей в структуру типа image_t.

\param triangles [in] Массив цветов пикселей треугольников
\param ptrnSizes [in] Массив с данными о ширине и высоте прямоугольных областей, в которые вписаны треугольники triangles
\param images [in] Массив структур типа image_t для сохранения информации о треугольниках
\param count [in] Количество треугольников 
*/
int rawToImage (nm32s* triangles, Size* ptrnSizes, image_t* images, int count);

#endif

/**
Функция printPattern выполняет вывод на экран паттерны для треугольников в виде псевдографики.

\param patterns [in] Массив паттернов треугольников
\param triangles [in] Массив цветов треугольников (не используется)
\param count [in] Количество паттернов
*/
void printPattern(Pattern* patterns, int count);

/**
Функция extractTriangleByPattern затирает пиксели, находящиеся вне шаблона.

\param patterns [in] Массив паттернов треугольников
\param triangle [in] Массив, содержащий закрашенный треугольник 
\param count [in] Количество паттернов
*/
void extractTriangleByPattern(Pattern* patterns, nm32s *triangle, int count);

void cropRectangle(nm32s *triangle, int count);

/**
Функция copyTriangleTo32x32 помещает результирующий треугольник размером 
ptrnSizes.height * ptrnSizes.width в область 32*32 для 
последующего сравнения с эталоном.

\param patterns [in] Массив паттернов треугольников
\param ptrnPoints [in] Массив с данными о координатах x и y прямоугольных областей, в которые вписаны треугольники triangles, в паттернах
\param ptrnSizes [in] Массив с данными о ширине и высоте прямоугольных областей, в которые вписаны треугольники triangles
\pSrcTriangle [in] Массив закрашенных входных треугольников, соответствующих параметрам ptrnPoints и ptrSizes 
\pDstTriangle [in] Массив закрашенных выходных треугольников, вписанных в область
размером 32*32
\param count [in] Количество треугольников 
*/
void copyTriangleTo32x32(Pattern* patterns, Vector2* ptrnPoints, Size* ptrnSizes, nm32s* pSrcTriangle, nm32s* pDstTriangle,  int count);

//Массив данных о треугольниках (координаты вершин, текстурные координаты и т.д.)
SECTION(".data_imu0") TrianglesInfo triangles;

// Паттерны треугольников. Паттерн представляет собой массив элементов из WIDTH_PTRN=32 строк и HEIGHT_PTRN=32 столбцов.
// Одна строка матрицы состоит из 32 элементов по 2 бита на элемент => каждая строка занимает 64 бита.
// Всего паттерн (WIDTH_PTRN * HEIGHT_PTRN / 16) intов на один треугольник
// 0-ой int - младшая половина 64-битной 0-ой строки паттерна
// 1-ой int - младшая половина 64-битной 0-ой строки паттерна
// 2-ой int - младшая половина 64-битной 1-ой строки паттерна
// 3-ой int - младшая половина 64-битной 2-ой строки паттерна
// 0-ая строка паттерна - верхняя строка
SECTION(".data_imu0") Pattern patterns [TRIANGLE_AMOUNT] = { 
//0,0 0,3 3,3
{0x00000015, 0x00000000,
0x00000005, 0x00000000,
0x00000001, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000},

//0,0 0,7 7,7
{0x00001555, 0x00000000,
0x00000555, 0x00000000,
0x00000155, 0x00000000,
0x00000055, 0x00000000,
0x00000015, 0x00000000,
0x00000005, 0x00000000,
0x00000001, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000},

//0,0 0,15 15,15
{0x15555555, 0x00000000,
0x05555555, 0x00000000,
0x01555555, 0x00000000,
0x00555555, 0x00000000,
0x00155555, 0x00000000,
0x00055555, 0x00000000,
0x00015555, 0x00000000,
0x00005555, 0x00000000,
0x00001555, 0x00000000,
0x00000555, 0x00000000,
0x00000155, 0x00000000,
0x00000055, 0x00000000,
0x00000015, 0x00000000,
0x00000005, 0x00000000,
0x00000001, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000},

//0,0 0,31 31,31
{0x55555555, 0x15555555,
0x55555555, 0x05555555,
0x55555555, 0x01555555,
0x55555555, 0x00555555,
0x55555555, 0x00155555,
0x55555555, 0x00055555,
0x55555555, 0x00015555,
0x55555555, 0x00005555,
0x55555555, 0x00001555,
0x55555555, 0x00000555,
0x55555555, 0x00000155,
0x55555555, 0x00000055,
0x55555555, 0x00000015,
0x55555555, 0x00000005,
0x55555555, 0x00000001,
0x55555555, 0x00000000,
0x15555555, 0x00000000,
0x05555555, 0x00000000,
0x01555555, 0x00000000,
0x00555555, 0x00000000,
0x00155555, 0x00000000,
0x00055555, 0x00000000,
0x00015555, 0x00000000,
0x00005555, 0x00000000,
0x00001555, 0x00000000,
0x00000555, 0x00000000,
0x00000155, 0x00000000,
0x00000055, 0x00000000,
0x00000015, 0x00000000,
0x00000005, 0x00000000,
0x00000001, 0x00000000,
0x00000000, 0x00000000},

//0,10 15,30 26,5
{0x50000000, 0x00000000, 
0x54000000, 0x00000000, 
0x54000000, 0x00000001, 
0x55000000, 0x00000001, 
0x55400000, 0x00000005, 
0x55400000, 0x00000005, 
0x55500000, 0x00000015, 
0x55540000, 0x00000015, 
0x55540000, 0x00000015, 
0x55550000, 0x00000055, 
0x55554000, 0x00000055, 
0x55554000, 0x00000155, 
0x55555000, 0x00000155, 
0x55555400, 0x00000555, 
0x55555400, 0x00000555, 
0x55555500, 0x00001555, 
0x55555500, 0x00001555, 
0x55555540, 0x00001555, 
0x55555550, 0x00005555, 
0x55555550, 0x00005555, 
0x55555555, 0x00015555, 
0x55555540, 0x00015555, 
0x55540000, 0x00055555, 
0x50000000, 0x00055555, 
0x00000000, 0x00055540, 
0x00000000, 0x00000000, 
0x00000000, 0x00000000, 
0x00000000, 0x00000000, 
0x00000000, 0x00000000, 
0x00000000, 0x00000000, 
0x00000000, 0x00000000, 
0x00000000, 0x00000000},
};

int counter = 0;

template<class T> inline T* myMallocT() {
	T* result = (T*)halMalloc32(sizeof32(T));
	if (result == 0) throw counter;
	counter++;
	return result;
}

template<class T> inline T* myMallocT(int count) {
	T* result = (T*)halMalloc32(count * sizeof32(T));
	if (result == 0) throw counter;
	counter++;
	return result;
}


//Глобальный контекст nm1
SECTION(".data_imu0") NMGL_Context_NM1 context;
SECTION(".data_imu0") NMGL_Context_NM1 *NMGL_Context_NM1::context;

SECTION(".text_demo3d") 
int main ()
{
    NMGL_Context_NM1 *cntxt;
    NMGL_Context_NM1::bind(&context);
    cntxt = NMGL_Context_NM1::getContext();
    cntxt->texState.init();
    
    //Массивы растеризованных и закрашенных треугольников
    nm32s pDstTriangle [WIDTH_PTRN * HEIGHT_PTRN * 1]; 
    nm32s pDstTriangle_32x32 [WIDTH_PTRN * HEIGHT_PTRN * 1]; 
    
  //Информация о размещении видимой части треугольников в сегменте
  Vector2 ptrnInnPoints [TRIANGLE_AMOUNT];
  Size ptrnSizes [TRIANGLE_AMOUNT];

  int i = 0;
  int j = 0;

  clock_t clkStart=0;
  clock_t clkEnd=0;
  clock_t diff = 0;

  printf ("\nStart smooth shading test...\n");

  ptrnInnPoints[0].x = 0;
  ptrnInnPoints[0].y = 0;
  ptrnSizes[0].width = 4;
  ptrnSizes[0].height = 3;

  ptrnInnPoints[1].x = 0;
  ptrnInnPoints[1].y = 0;
  ptrnSizes[1].width = 8;
  ptrnSizes[1].height = 7;

  ptrnInnPoints[2].x = 0;
  ptrnInnPoints[2].y = 0;
  ptrnSizes[2].width = 16;
  ptrnSizes[2].height = 15;

  ptrnInnPoints[3].x = 0;
  ptrnInnPoints[3].y = 0;
  ptrnSizes[3].width = 32;
  ptrnSizes[3].height = 31;

  ptrnInnPoints[4].x = 0;
  ptrnInnPoints[4].y = 0;
  ptrnSizes[4].width = 26;
  ptrnSizes[4].height = 25;

  //window coordinates of next triangles:
  //0,0 0,3 3,3
  //0,0 0,7 7,7
  //0,0 0,15 15,15
  //0,0 0,31 31,31
  //0,10 15,30 26.5
  float x0[TRIANGLE_AMOUNT] = {384.0f, 384.0f, 384.0f, 384.0f, 384.0f};
  float y0[TRIANGLE_AMOUNT] = {384.0f, 384.0f, 384.0f, 384.0f, 374.0f};
  float x1[TRIANGLE_AMOUNT] = {384.0f, 384.0f, 384.0f, 384.0f, 399.0f};
  float y1[TRIANGLE_AMOUNT] = {381.0f, 377.0f, 369.0f, 353.0f, 354.0f};
  float x2[TRIANGLE_AMOUNT] = {387.0f, 391.0f, 399.0f, 415.0f, 410.0f};
  float y2[TRIANGLE_AMOUNT] = {381.0f, 377.0f, 369.0f, 353.0f, 379.0f};

  int c0[TRIANGLE_AMOUNT] = {0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000};//Red
  int c1[TRIANGLE_AMOUNT] = {0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00};//Green
  int c2[TRIANGLE_AMOUNT] = {0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff, 0xff0000ff};//Blue

  cntxt->texState.segX0 = 384;
  cntxt->texState.segY0 = 256;
  cntxt->texState.segWidth = 128;

  cntxt->smallColorBuff.mData = (void*)81920; // address of "left lower" corner of segment 
  //("left lower" pixel) in memory. Axis y points down.
  int pointInImage[TRIANGLE_AMOUNT] = {16000, 15488, 14464, 12416, 12544}; // offsets of triangle window's "left lower" corner in segment
  nm32s* PROI[TRIANGLE_AMOUNT] = {0, 0, 0, 0, 0}; // addresses of triangle window's "left lower" corner

  for (i = 0; i < TRIANGLE_AMOUNT; i++)
  {
    PROI[i] = (int*)cntxt->smallColorBuff.mData + pointInImage[i]; //{97920, 97408, 96384, 94336, 94464}
  }

  float z[TRIANGLE_AMOUNT] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}; //minus (z in camera space)

  unsigned int pixelCount[TRIANGLE_AMOUNT] = {8, 32, 128, 512, 298};


  int refId = 0;

  // Performance tests: TRIANGLE_AMOUNT triangles
  for (i = 0; i < TRIANGLE_AMOUNT; ++i){
    double corr = -1.0;		// -1.0 - correctness is not calculated
    triangles.x0 = &x0[i];
    triangles.y0 = &y0[i];
    triangles.x1 = &x1[i];
    triangles.y1 = &y1[i];
    triangles.x2 = &x2[i];
    triangles.y2 = &y2[i];

    triangles.z0 = &z[i];
    triangles.z1 = &z[i];
    triangles.z2 = &z[i];

    triangles.c0 = &c0[i];
    triangles.c1 = &c1[i];
    triangles.c2 = &c2[i];

    Vector2* curPtrnInnPoint = &ptrnInnPoints[i];
    Size* curPtrnSize = &ptrnSizes[i];
    nm32s** curPROI = &PROI[i];
    Pattern* curPattern = &patterns[i];

    //shade triangle
    clock_t start_time = 0;
    start_time = clock();
    triangleShadeSmooth(curPattern, &triangles, curPROI, curPtrnInnPoint, curPtrnSize, pDstTriangle, 1);
    clock_t end_time = 0;
    end_time = clock();


    // Correctness tests: the last set of coordinates 
    // with scenario texcoords
    if (i == TRIANGLE_AMOUNT - 1){
      copyTriangleTo32x32(curPattern, curPtrnInnPoint, curPtrnSize, pDstTriangle, pDstTriangle_32x32, 1);
      extractTriangleByPattern(curPattern, pDstTriangle_32x32, 1);
#ifndef __NM__
      // Save to bmp file on x86    
      // before correction test because it corrupts images
      char filename[256];
      char filename_ref[256];
      Size win_32x32;
      win_32x32.width = WIDTH_PTRN;
      win_32x32.height= HEIGHT_PTRN;

      snprintf(filename, 256, "%i_res.%s", refId + 1, "bmp");
      snprintf(filename_ref, 256, "%i_ref.%s", refId + 1, "bmp");
      rawToImage(pDstTriangle_32x32, &win_32x32, result_images, 1);
      saveToBmp (32, result_images[0], filename);
      rawToImage(references[refId], &win_32x32, result_images, 1);
      saveToBmp (32, result_images[0], filename_ref);
#endif //__NM__        
      corr = compareImages(pDstTriangle_32x32, references[refId], WIDTH_PTRN, HEIGHT_PTRN);
    }

    // Output result
    printf("\t%10i/", (int)(end_time - start_time));
    printf("%6i/", (int)(end_time - start_time)/pixelCount[i]);
    if (corr < 0){
      printf("-", (double)(end_time - start_time));
    } else {
      printf("%8.6f", corr * 100);
    }
  }
  printf ("\nEnd smooth shading test...\n");
  return 0;
}

void printPattern(Pattern* patterns, int count)
{
  int i = 0;
  int j = 0;

  for (i = 0; i < count; i++)
  {
    for (int j = 0; j < 32; j++)
    {
      //sprintf("i = %d %x \n", i, context->ppSrcPackPtrns[0][i]);
      for (int k = 0; k < 32; k++)
        if (((nm64s*)patterns[i])[j] & ((nm64s)1 << (k * 2)))
          printf("1 ");
        else
          printf("0 ");
      // if (i & 1)
      printf("\n\r");
    }
  }
}

void extractTriangleByPattern(Pattern* patterns, nm32s *triangle, int count)
{
  int i = 0;
  int j = 0;

  for (i = 0; i < count; i++)
  {
    for (int j = 0; j < 32; j++)
    {
      //sprintf("i = %d %x \n", i, context->ppSrcPackPtrns[0][i]);
      //printf("%016x\n",((nm64s*)patterns[i])[j]);
      for (int k = 0; k < 32; k++)
        if (((nm64s*)patterns[i])[j] & ((nm64s)1 << (k * 2))){
          // Keep this pixel
        } else {
          // Erase this pixel
          triangle[32 * j + k] = 0xff000000;
        }
    }
  }
}
void cropRectangle(nm32s *triangle, int count)
{
  int i = 0;
  int j = 0;

  for (i = 0; i < count; i++)
  {
    for (int j = 0; j < 32; j++)
    {
      triangle[32 * j + 31] = 0xff000000;	// Black last column
      triangle[32 * 31 + j] = 0xff000000; // Black last row
    }
  }
}

#ifndef __NM__

int rawToImage (nm32s* triangles, Size* ptrnSizes, image_t* images, int count)
{

  int i = 0;
  int srcRow = 0;
  int dstRow = 0;
  int col = 0;

  nm32s* triangle = triangles;

  for (i = 0; i < count; i++)
  {
    int width = ptrnSizes[i].width;
    int height = ptrnSizes[i].height;
    nm32s pixel_pos = 0;
    nm32s pixel_value = 0;

    images[i].width = width;
    images[i].height = height;
    images[i].format = RGBA;
    images[i].type = UNSIGNED_BYTE;
    images[i].alignment = 1;

    //copy pixels from triangles to image
    images[i].pixels = (unsigned char*) malloc(width*height*4); //TODO: nmc char type has size of 4 bytes. 
    //Perhaps it would be better to use UNSIGNED_INT_8_8_8_8
    for (srcRow = height-1, dstRow = 0; srcRow >= 0; srcRow--, dstRow++)
    {
      for (col = 0; col < width; col++)
      {
        pixel_pos = (dstRow*width + col)*4;
        pixel_value = triangle[srcRow*width + col];
        //0xARGB t0 0xRGBA
        ((unsigned char*)images[i].pixels)[pixel_pos] = (pixel_value & 0x00ff0000) >> 16;
        ((unsigned char*)images[i].pixels)[pixel_pos + 1] = (pixel_value & 0x0000ff00) >> 8;
        ((unsigned char*)images[i].pixels)[pixel_pos + 2] = (pixel_value & 0x000000ff);
        ((unsigned char*)images[i].pixels)[pixel_pos + 3] = (pixel_value & 0xff000000) >> 24;
      }
    }

    triangle += ptrnSizes[i].height * ptrnSizes[i].width;

    // for (dstRow = 0; dstRow < height; dstRow++)
    // {
    // for (col = 0; col < width; col++)
    // {
    // printf ("%08x ", ((int*)images[i].pixels)[dstRow*width + col]);
    // }
    // printf ("\n\n");            
    // }

    // printf ("\n\n");   
  }



  return 0;
}

#endif //__NM__

void copyTriangleTo32x32(Pattern* patterns, Vector2* ptrnPoints, Size* ptrnSizes, nm32s* pSrcTriangle, nm32s* pDstTriangle,  int count){
  long long int temp;
  nm32s* src = pSrcTriangle;
  nm32s* dst = pDstTriangle;
  for(int c=0;c<count;c++){

    nm32s* pDst = (nm32s*)(dst + ptrnPoints[c].y * WIDTH_PTRN);
    int width = ptrnSizes[c].width;

    for(int y = 0; y < ptrnSizes[c].height; y++){

      nm32s* pSrcRow = src + y * ptrnSizes[c].width;
      nm32s* pDstRow = pDst + y * WIDTH_PTRN;
      if (ptrnPoints[c].x > 0) {
        pDstRow = (nm32s*)(pDstRow + ptrnPoints[c].x);
      }

      for(int x = 0; x < width; x++){
        pDstRow[x] = pSrcRow[x];
      }
    }
    src += ptrnSizes[c].height * ptrnSizes[c].width;
    dst += WIDTH_PTRN * HEIGHT_PTRN;
  }
}

