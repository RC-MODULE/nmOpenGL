#ifndef __DEMO3D_COMMON_H_INCLUDED__
#define __DEMO3D_COMMON_H_INCLUDED__
#include "nmtype.h"
#include "nmgltype.h"
#include "nmsynchro.h"
#include "ringbuffer.h"
#include "pattern.h"
//#include "testsize.h"

#ifdef __GNUC__
	#define setHeap(n) nmc_malloc_set_heap(n) 
#else
	#define setHeap(n)
#endif

//#define USED_OLD_POLYGONS
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
#define NMGL_SIZE 1024
#define POLYGONS_SIZE 256

#define SMALL_SIZE 	  16
#define SIZE_BANK	0x8000

#define BLACK 0x00000000
#define DARK_GRAY 0x44444444
#define GRAY 0x88888888
#define LIGHT_GRAY 0xCCCCCCCC
#define WHITE 0xFFFFFFFF

#define COUNT_POLYGONS_BUFFER 256
#define COUNT_IMAGE_BUFFER 8
//#define COUNT_IMAGE_BUFFER 2


struct Vector2 {
	int x;
	int y;
};

struct Size {
	int width;
	int height;
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
struct DataForNmpu1 {
	int x0[POLYGONS_SIZE];
	int y0[POLYGONS_SIZE];
	int x1[POLYGONS_SIZE];
	int y1[POLYGONS_SIZE];
	int x2[POLYGONS_SIZE];
	int y2[POLYGONS_SIZE];
	int z[POLYGONS_SIZE];
	int crossProducts[POLYGONS_SIZE];

	int color[4 * POLYGONS_SIZE];

	int count;
	int dummy[15];

	DataForNmpu1() : count(0) {};
};


/**
 *  (устаревшая)Структура для передачи полигонов от nmpu0 к nmpu1.
 *  Предполагается, что точки (x0, y0), (x1, y1) и (x2, y2) расположены в порядке возрастания y, 
 *  т.е. точка (x0, y0) обладает наименьшим y
 *  Полигон должен вписываться в квадрат 32*32 пикселей
 */
struct PolygonsOld {
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

	int color[4 * POLYGONS_SIZE];

	int z[POLYGONS_SIZE];

	int count;
	int iSeg;
	int dummy[14];

	PolygonsOld() : count(0) {
		
	}
};

#ifdef USED_OLD_POLYGONS
typedef PolygonsOld Polygons;
#else
typedef DataForNmpu1 Polygons;
#endif // USED_OLD_POLYGONS

typedef HalRingBufferData<Polygons, COUNT_POLYGONS_BUFFER> PolygonsArray;
typedef HalRingBufferConnector<Polygons, COUNT_POLYGONS_BUFFER> PolygonsConnector;

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
	int offset;
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
