#include <cstdio>
#include <cstdlib>
#include "nmtype.h"
#include "nmgldef.h"
#include "nmgltex_common.h"
#include "nmgltex_nm0.h"
#include "demo3d_nm0.h"
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

using namespace nm0_version;

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
SECTION(".data_imu0") Pattern patterns [1] = { 
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

SECTION(".data_imu0") NMGL_Context_NM0 *NMGL_Context_NM0::context;

SECTION(".text_demo3d") 
int main ()
{
	
	NMGL_Context_NM0 *test_cntxt;
	//NMGLSynchroData synchroData;
	//synchroData.init();
	//NMGL_Context_NM0::create(&synchroData);	
	NMGL_Context_NM0::create();	
	test_cntxt = NMGL_Context_NM0::getContext();
	
    //Массивы растеризованных и закрашенных треугольников
	nm32s pSrcTriangle[WIDTH_PTRN * HEIGHT_PTRN * 1];
	nm32s pDstTriangle [WIDTH_PTRN * HEIGHT_PTRN * 1]; 
    
	
	//Информация о размещении видимой части треугольников в сегменте
	Size ptrnSizes[1];
    
    int i = 0;
    int j = 0;
	
	clock_t clkStart=0;
	clock_t clkEnd=0;
	clock_t diff = 0;

	printf ("\nStart shading test...\n");

    ptrnSizes[0].width = 32;
    ptrnSizes[0].height = 32;
  
    float x0[TRIANGLE_AMOUNT] = {0.0f, 0.0f,  0.0f,  0.0f,  0.0f};
    float y0[TRIANGLE_AMOUNT] = {0.0f, 0.0f,  0.0f,  0.0f, 32.0f - 10.0f};
    float x1[TRIANGLE_AMOUNT] = {0.0f, 0.0f,  0.0f,  0.0f, 15.0f};
    float y1[TRIANGLE_AMOUNT] = {3.0f, 7.0f, 15.0f, 31.0f, 32.0f - 30.0f};
    float x2[TRIANGLE_AMOUNT] = {3.0f, 7.0f, 15.0f, 31.0f, 26.0f};
    float y2[TRIANGLE_AMOUNT] = {0.0f, 0.0f,  0.0f,  0.0f, 32.0f - 5.0f};
   
    v4nm32s c0[TRIANGLE_AMOUNT] = {0};
    v4nm32s c1[TRIANGLE_AMOUNT] = {0};
    v4nm32s c2[TRIANGLE_AMOUNT] = {0};

    //TODO:R,G,B,A order is used in c0,c1,c2. May be it will be necessary to use B,G,R,A order 
    nm32s* curC = (nm32s*)c0;
    for (int i = 0; i < TRIANGLE_AMOUNT; i++) {
        curC[0] = (nm32s)255;//r
        curC[1] = (nm32s)0;//g
        curC[2] = (nm32s)0;//b
        curC[3] = (nm32s)255;//a
        curC += 4;
    }
    
    curC = (nm32s*)c1;
    for (int i = 0; i < TRIANGLE_AMOUNT; i++) {
        curC[0] = (nm32s)0;//r
        curC[1] = (nm32s)255;//g
        curC[2] = (nm32s)0;//b
        curC[3] = (nm32s)255;//a
        curC += 4;
    }

    curC = (nm32s*)c2;
    for (int i = 0; i < TRIANGLE_AMOUNT; i++) {
        curC[0] = (nm32s)0;//r
        curC[1] = (nm32s)0;//g
        curC[2] = (nm32s)255;//b
        curC[3] = (nm32s)255;//a
        curC += 4;
    }

    float z[TRIANGLE_AMOUNT] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}; //minus (z in camera space)
    unsigned int pixelCount[TRIANGLE_AMOUNT] = {8, 32, 128, 512, 298}; //minus (z in camera space)
    
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

      clock_t start_time = 0;
      start_time = clock();
      triangleShadeSmooth(&triangles, pDstTriangle, 1);
      clock_t end_time = 0;
      end_time = clock();

      // Correctness tests: the last set of coordinates 
      // with scenario texcoords
      if (i == TRIANGLE_AMOUNT - 1){
        extractTriangleByPattern(patterns, pDstTriangle, 1);
#ifndef __NM__
        // Save to bmp file on x86    
        // before correction test because it corrupts images
        char filename[256];
        char filename_ref[256];
        Size win_32x32;
        win_32x32.width = WIDTH_PTRN;
        win_32x32.height= HEIGHT_PTRN;

        snprintf(filename, 256, "%i_res.%s", refId + 1, "bmp");
        rawToImage(pDstTriangle, ptrnSizes, result_images, 1);
        saveToBmp (32, result_images[0], filename);

        snprintf(filename_ref, 256, "%i_ref.%s", refId + 1, "bmp");
        rawToImage(references[refId], &win_32x32, result_images, 1);
        saveToBmp (32, result_images[0], filename_ref);
#endif //__NM__        
        corr = compareImages(pDstTriangle, references[refId], WIDTH_PTRN, HEIGHT_PTRN);
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
    printf ("\nEnd shading test...\n");
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
