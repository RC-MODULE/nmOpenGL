#ifndef __DEMO3D_COMMON_H_INCLUDED__
#define __DEMO3D_COMMON_H_INCLUDED__
#include "nmtype.h"
#include "nmgltype.h"
#include "nmsynchro.h"
#include "ringbuffer.h"

#ifdef __GNUC__
	#define setHeap(n) nmc_malloc_set_heap(n) 
#else
	#define setHeap(n)
#endif

//#define PROFILER0
//#define PROFILER1

#ifdef __GNUC__
#define SECTION(sec) __attribute__((section(sec)))
#else
#define SECTION(sec)
#endif // __GNUC__

typedef nm16s rgb565;
typedef v4nm8s rgb8888;

//Возможные значения ширины или высоты: 768,960,1024,1152
#define WIDTH_IMAGE 768
#define HEIGHT_IMAGE 768

#define MAX_LIGHTS 2

#define WIDTH_SEG 128
#define HEIGHT_SEG 128

#ifdef TEXTURE_ENABLED
#define NMGL_SIZE 256 
#else //TEXTURE_ENABLE
#define NMGL_SIZE 1024 
#endif //TEXTURE_ENABLED

#define POLYGONS_SIZE NMGL_SIZE

#define BLACK 0x00000000
#define DARK_GRAY 0x44444444
#define GRAY 0x88888888
#define LIGHT_GRAY 0xCCCCCCCC
#define WHITE 0xFFFFFFFF

#define COUNT_POLYGONS_BUFFER 128
#define COUNT_IMAGE_BUFFER 8

#define ZBUFF_MAX 0x7FFFFFFF
#define ZBUFF_MAX_15s 0x7FFF
#define ZBUFF_INIT_VALUE ZBUFF_MAX

#define MAX_SIDE_POLYGON 32
#define HEIGHT_PTRN   MAX_SIDE_POLYGON
#define WIDTH_PTRN    MAX_SIDE_POLYGON
#define SMALL_SIZE 	  16
#define SIZE_BANK	0x8000
#define OFFSETS 	  WIDTH_PTRN
#define AMOUNT_ANGLES (2*WIDTH_PTRN + 2*HEIGHT_PTRN)
#define NPATTERNS 	  AMOUNT_ANGLES * OFFSETS * 2

typedef int Pattern[WIDTH_PTRN * HEIGHT_PTRN / 16];

struct PatternsArray {
	Pattern ptrns[NPATTERNS];
	int table_dydx[(2 * WIDTH_PTRN) * (HEIGHT_PTRN + 2)];
};


struct Rectangle {
	int x;
	int y;
	int width;
	int height;
};

/**
 *  Структура для передачи полигонов от nmpu0 к nmpu1.
 *  Предполагается, что точки (x0, y0), (x1, y1) и (x2, y2) расположены в порядке возрастания y, 
 *  т.е. точка (x0, y0) обладает наименьшим y
 *  Полигон должен вписываться в квадрат 32*32 пикселей
 */
struct Polygons {
	int numbersPattrns01[POLYGONS_SIZE];
	int numbersPattrns12[POLYGONS_SIZE];
	int numbersPattrns02[POLYGONS_SIZE];
	int ptrnSizesOf32_01[POLYGONS_SIZE];
	int ptrnSizesOf32_02[POLYGONS_SIZE];
	int pointInImage[POLYGONS_SIZE];
	int offsetsX[POLYGONS_SIZE];
	int offsetsY[POLYGONS_SIZE];
	int widths[POLYGONS_SIZE];
	int heights[POLYGONS_SIZE];
#ifdef TEXTURE_ENABLED 
	float x0[POLYGONS_SIZE];
	float y0[POLYGONS_SIZE];
	float x1[POLYGONS_SIZE];
	float y1[POLYGONS_SIZE];
	float x2[POLYGONS_SIZE];
	float y2[POLYGONS_SIZE];
	float zEye[POLYGONS_SIZE];
	
	float s0[POLYGONS_SIZE];
	float t0[POLYGONS_SIZE];
	float s1[POLYGONS_SIZE];
	float t1[POLYGONS_SIZE];
	float s2[POLYGONS_SIZE];
	float t2[POLYGONS_SIZE];
#endif //TEXTURE_ENABLED 
	int color[4 * POLYGONS_SIZE];

	int z[POLYGONS_SIZE];

	//Rectangle ptrnsWindow[POLYGONS_SIZE];
	int count;
	int dummy[15];
	// int dummy[2];

	Polygons() : count(0) {
		
	}
};

typedef HalRingBufferData<Polygons, COUNT_POLYGONS_BUFFER> PolygonsArray;
typedef HalRingBufferConnector<Polygons, COUNT_POLYGONS_BUFFER> PolygonsConnector;

typedef int ImageRGB8888[WIDTH_IMAGE * HEIGHT_IMAGE];
typedef int DepthImage32[WIDTH_IMAGE * HEIGHT_IMAGE];
typedef HalRingBufferData<ImageRGB8888, COUNT_IMAGE_BUFFER> ImageData;
typedef HalRingBufferConnector<ImageRGB8888, COUNT_IMAGE_BUFFER> ImageConnector;


//typedef int matrix[16];
typedef struct s_mat4nm32f{
	float matr[16];
} mat4nm32f;

typedef struct s_v3nm32f {
	float v0;
	float v1;
	float v2;
} v3nm32f;

struct Array {
	const void* pointer;
	NMGLint size;
	NMGLint stride;
	NMGLenum type;
	NMGLboolean enabled;
	int dummy;
};

struct WindowInfo {
	int x0[20];
	int y0[20];
	int x1[20];
	int y1[20];
	float x0_f[20];
	float y0_f[20];
	float x1_f[20];
	float y1_f[20];
	int segmentWidth;
	int segmentHeight;
	int imageWidth;
	int imageHeight;	
	float viewportMulX;
	float viewportMulY;
	float viewportMulZ;
	float viewportAddX;
	float viewportAddY;
	float viewportAddZ;
	int nColumns;
	int nRows;
};

#endif
