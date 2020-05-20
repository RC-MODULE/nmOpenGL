#include "nmtype.h"
#include "nmgldef.h"
#include "textureTriangle.h"
#include <cstdio>
#include "tex_common.h"
#include "image.h"
#include "nmgltex_nm1.h"
#include "demo3d_nm1.h"
#include <cstdlib>
#include "nmplv/vSupport.h"
#include "malloc32.h"

#ifndef __NM__
#include "bmp.h"
#endif

#ifdef __GNUC__
#define SECTION(sec) __attribute__((section(sec)))
#else
#define SECTION(sec)
#endif // __GNUC__

#define TRIANGLE_AMOUNT 2

//Массив текстур в виде структур типа image_t
#define TEXTURE_AMOUNT 9

extern "C" image_t image_256_256;
extern "C" image_t image_128_128;
extern "C" image_t image_64_64;
extern "C" image_t image_32_32;
extern "C" image_t image_16_16;
extern "C" image_t image_8_8;
extern "C" image_t image_4_4;
extern "C" image_t image_2_2;
extern "C" image_t image_1_1;

image_t textures [TEXTURE_AMOUNT] = {
    image_256_256,
    image_128_128,
    image_64_64,
    image_32_32,
    image_16_16,
    image_8_8,
    image_4_4,
    image_2_2,
    image_1_1
};

//массив изображений после текстурирования
image_t result_images [TRIANGLE_AMOUNT]; 

//Глобальный контекст nm1
NMGL_Context_NM1 cntxt; 

/**
Функция imageToTexImage выполняет копирование данных из структуры типа image_t в структуру типа TexImage2D. Функция заполняет поля internalFormat, pixels, width, height.

\param image [in] Структура типа image_t.
\param texImage [in] Структура типа TexImage2D, которая должна быть заполнена данными из image.
*/
int imageToTexImage(image_t* image, TexImage2D* texImage);

/**
Функция rawToImage выполняет преобразование массива цветов пикселей в структуру типа image_t.

\param triangles [in] Массив цветов пикселей треугольников
\param windows [in] Массив с данными о треугольниках в массиве triangles
\param images [in] Массив структур типа image_t для сохранения информации о треугольниках
\param count [in] Количество треугольников 
*/
int rawToImage (nm32s* triangles, Rectangle* windows, image_t* images, int count);


/**
Функция printPattern выполняет вывод на экран паттерны для треугольников в виде псевдографики.

\param patterns [in] Массив паттернов треугольников
\param triangles [in] Массив цветов треугольников (не используется)
\param count [in] Количество паттернов
*/
void printPattern(Pattern* patterns, int count);

// Паттерны треугольников. Паттерн представляет собой массив элементов из WIDTH_PTRN=32 строк и HEIGHT_PTRN=32 столбцов.
// Одна строка матрицы состоит из 32 элементов по 2 бита на элемент => каждая строка занимает 64 бита.
// Всего паттерн (WIDTH_PTRN * HEIGHT_PTRN / 16) intов на один треугольник
// 0-ой int - младшая половина 64-битной 0-ой строки паттерна
// 1-ой int - младшая половина 64-битной 0-ой строки паттерна
// 2-ой int - младшая половина 64-битной 1-ой строки паттерна
// 3-ой int - младшая половина 64-битной 2-ой строки паттерна
// 0-ая строка паттерна - верхняя строка
Pattern patterns [TRIANGLE_AMOUNT] = { 
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

SECTION(".text_demo3d") 
int main ()
{
    //Массив данных о треугольниках (координаты вершин, текстурные координаты и т.д.)
    Triangles triangles;
    
    //Массивы растеризованных и закрашенных треугольников
    nm32s pSrcTriangle [WIDTH_PTRN * HEIGHT_PTRN * TRIANGLE_AMOUNT]; //
    nm32s pDstTriangle [WIDTH_PTRN * HEIGHT_PTRN * TRIANGLE_AMOUNT]; 
    
    //Массив значений цветов для 
    nm32s valueC [TRIANGLE_AMOUNT] = {0x0000ffff, 0x00ff00ff}; //0xBGRA
    
    //Информация о размещении видимой части треугольников в сегменте
    Rectangle windows [TRIANGLE_AMOUNT];
    
    int count = TRIANGLE_AMOUNT;
    int i = 0;
    int j = 0;

    windows[0].x = -1;
    windows[0].y = 0;
    windows[0].width = 26;
    windows[0].height = 25;
  
    windows[1].x = -1;
    windows[1].y = 0;
    windows[1].width = 26;
    windows[1].height = 25;

    //setup triangles parameters
    float x0[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float y0[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float x1[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float y1[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float x2[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float y2[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    
    float s0[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float t0[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float s1[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float t1[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float s2[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    float t2[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
   
    float z[TRIANGLE_AMOUNT] = {0.0f, 0.0f};
    
    triangles.x0 = x0;
    triangles.y0 = y0;
    triangles.x1 = x1;
    triangles.y1 = y1;
    triangles.x2 = x2;
    triangles.y2 = y2;
 
    triangles.s0 = s0;
    triangles.t0 = t0;
    triangles.s1 = s1;
    triangles.t1 = t1;
    triangles.s2 = s2;
    triangles.t2 = t2;
    
    triangles.z = z;
    triangles.size = TRIANGLE_AMOUNT;
 
    printf ("Start textureTriangle test...\n");

    //Активный текстурный модуль
    cntxt.texState.activeTexUnitIndex = 0;
    unsigned int activeTexUnitIndex = cntxt.texState.activeTexUnitIndex;
    
    //Текстурный объект, привязанный к активному текстурному модулю
    cntxt.texState.texUnits[activeTexUnitIndex].boundTexObject = &cntxt.texState.texObjects[0];
    TexObject* boundTexObject = cntxt.texState.texUnits[activeTexUnitIndex].boundTexObject;

    //fill TexImages in texture context
    for (i = 0; i < TEXTURE_AMOUNT; i++)
    {
       imageToTexImage(&textures[i], &boundTexObject->texImages2D[i]);
    }

    // printPattern(patterns, pSrcTriangle, TRIANGLE_AMOUNT);
    
    //fill triangles with colors
    mMulCVxN_2s_RGB8888(patterns, windows, (v4nm8s*)valueC, pSrcTriangle, TRIANGLE_AMOUNT);
    
    //texture triangles
    textureTriangle(patterns, &triangles, windows, pSrcTriangle, pDstTriangle, count);

    //convert result to image_t to compare with etalon    
    rawToImage(pDstTriangle, windows, result_images, count);
    
    //compare with etalon

#ifndef __NM__

    char* fileNames [TRIANGLE_AMOUNT] = {"./result_1.bmp", "./result_2.bmp"};

    for (i = 0; i < count; i++)
    {
        saveToBmp (32, result_images[i], fileNames[i]);
    }
#endif //__NM__        
    
    
    return 0;
}

int imageToTexImage(image_t* image, TexImage2D* texImage)
{
    
    if (image->type != UNSIGNED_BYTE)
    {
        printf ("%s %s %d: Unsupported texture pixel type",__FILE__, __func__, __LINE__);
        return -1;
    }
    
    switch (image->format)
    {
        case RGB:
            texImage->internalformat = NMGL_RGB;
            break;
        
        case RGBA:
            texImage->internalformat = NMGL_RGBA;
            break;
            
        case ARGB:
            printf ("%s %s %d: Unsupported NMGL pixel format type (ARGB)\n", __FILE__, __func__, __LINE__);
            return -1;

        case ALPHA:
            texImage->internalformat = NMGL_ALPHA;
            break;

        case LUMINANCE:
            texImage->internalformat = NMGL_LUMINANCE;
            break;

        case LUMINANCE_ALPHA:
            texImage->internalformat = NMGL_LUMINANCE_ALPHA;
            break;

        default:
            printf ("%s %s %d: Unsupported NMGL pixel format type (unknown code)\n\n",  __FILE__, __func__, __LINE__);
            return -1;
    }
    
    texImage->pixels = (void*)image->pixels;
    texImage->width = image->width;
    texImage->height = image->height;
    
    return 0;
}

int rawToImage (nm32s* triangles, Rectangle* windows, image_t* images, int count)
{
    
    int i = 0;
    int srcRow = 0;
    int dstRow = 0;
    int col = 0;

    nm32s* triangle = triangles;
    
    for (i = 0; i < count; i++)
    {
        int width = windows[i].width;
        int height = windows[i].height;
        nm32s pixel_pos = 0;
        nm32s pixel_value = 0;
        
        images[i].width = width;
        images[i].height = height;
        images[i].format = RGBA;
        images[i].type = UNSIGNED_BYTE;
        
        //copy pixels from triangles to image
        images[i].pixels = (unsigned char*) malloc(width*height*4); //TODO: nmc char type has size of 4 bytes. 
                                                                    //Perhaps it would be better to use UNSIGNED_INT_8_8_8_8
        for (srcRow = height-1, dstRow = 0; srcRow >= 0; srcRow--, dstRow++)
        {
            for (col = 0; col < width; col++)
            {
                pixel_pos = (dstRow*width + col)*4;
                pixel_value = triangle[srcRow*width + col];
                ((unsigned char*)images[i].pixels)[pixel_pos] = (pixel_value & 0x0000ff00) >> 8;
                ((unsigned char*)images[i].pixels)[pixel_pos + 1] = (pixel_value & 0x00ff0000) >> 16;
                ((unsigned char*)images[i].pixels)[pixel_pos + 2] = (pixel_value & 0xff000000) >> 24;
                ((unsigned char*)images[i].pixels)[pixel_pos + 3] = (pixel_value & 0x000000ff);
            }
        }
        
        triangle += windows[i].height * windows[i].width;

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
            printf("\n");
		}
    }
}

