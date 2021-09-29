/*!
 *  \file demo3d_common.h
 *  \brief Заголовочный файл с информацией общей для nm0 и nm1
 *  \author Жиленков Иван
 */
#ifndef __DEMO3D_COMMON_H_INCLUDED__
#define __DEMO3D_COMMON_H_INCLUDED__
#include "nmgltype.h"
#include "ringbuffert.h"
#include "pattern.h"
#include "nmglmath.h"

#ifdef __GNUC__
	#define setHeap(n) nmc_malloc_set_heap(n) 
	#define getHeap() nmc_malloc_get_heap() 
#else
	#define setHeap(n)
	#define getHeap() 0 
#endif

#ifndef __GNUC__
	#define PROFILER_SIZE(n)
	#define nmprofiler_enable()
	#define nmprofiler_disable()
#endif


#define TRIANGULATION_ENABLED
//#define PROFILER0
//#define PROFILER1

#define ActiveTexObjectP cntxt->texState.texUnits[cntxt->texState.activeTexUnitIndex].boundTexObject
#ifdef __GNUC__
#define SECTION(sec) __attribute__((section(sec)))
#else
#define SECTION(sec) 
#endif // __GNUC__

typedef nm16s rgb565;
typedef v4nm8s rgb8888;

//Возможные значения ширины или высоты: 480,640,768,960,1024,1152
#define WIDTH_IMAGE 768
#define HEIGHT_IMAGE 768

#define MAX_LIGHTS 2

#define WIDTH_SEG 128
#define HEIGHT_SEG 128
#define NMGL_SIZE 1024
#define POLYGONS_SIZE 256

#define SMALL_SIZE 	  16
#define SIZE_BUFFER_NM1	0x4000

#define BLACK 0x00000000
#define DARK_GRAY 0x44444444
#define GRAY 0x88888888
#define LIGHT_GRAY 0xCCCCCCCC
#define WHITE 0xFFFFFFFF

#define COUNT_POLYGONS_BUFFER 4

/*struct TrianglePackage{
	int x0[POLYGONS_SIZE];
	int y0[POLYGONS_SIZE];
	int x1[POLYGONS_SIZE];
	int y1[POLYGONS_SIZE];
	int x2[POLYGONS_SIZE];
	int y2[POLYGONS_SIZE];
	int crossProducts[POLYGONS_SIZE];
};

struct LinePackage{
	int x0[POLYGONS_SIZE];
	int y0[POLYGONS_SIZE];
	int x1[POLYGONS_SIZE];
	int y1[POLYGONS_SIZE];
	int crossProducts[POLYGONS_SIZE];
};

struct PointPackage {
	int x0[POLYGONS_SIZE];
	int y0[POLYGONS_SIZE];
	int x1[POLYGONS_SIZE];
	int y1[POLYGONS_SIZE];
	int crossProducts[POLYGONS_SIZE];
};*/


/**
*  \brief Структура для передачи полигонов от nmpu0 к nmpu1.
*  Предполагается, что точки (x0, y0), (x1, y1) и (x2, y2) расположены в порядке возрастания y,
*  т.е. точка (x0, y0) обладает наименьшим y
*  \warning Каждый полигон должен вписываться в квадрат 32*32 пикселей.
*  \author Жиленков Иван
*/
struct DataForNmpu1 {
	int x0[POLYGONS_SIZE];
	int y0[POLYGONS_SIZE];
	int x1[POLYGONS_SIZE];
	int y1[POLYGONS_SIZE];
	int x2[POLYGONS_SIZE];
	int y2[POLYGONS_SIZE];
	int crossProducts[POLYGONS_SIZE];
	int z[POLYGONS_SIZE];
	int color[4 * POLYGONS_SIZE];

	// TEXTURING_PART
	float s0[POLYGONS_SIZE];
	float t0[POLYGONS_SIZE];
	float s1[POLYGONS_SIZE];
	float t1[POLYGONS_SIZE];
	float s2[POLYGONS_SIZE];
	float t2[POLYGONS_SIZE];
	float w0[POLYGONS_SIZE];
	float w1[POLYGONS_SIZE];
	float w2[POLYGONS_SIZE];
	// TEXTURING_PART

	int count;
	int dummy[15];

	DataForNmpu1() : count(0) {};
};

template <class T, int SIZE> class MyRingBufferConnector : public HalRingBufferConnector<T, SIZE> {
	int dummy;
public:
	HalRingBufferData<T, SIZE>* ringbufferDataPointer;

};

typedef HalRingBufferData<DataForNmpu1, COUNT_POLYGONS_BUFFER> PolygonsArray;
typedef MyRingBufferConnector<DataForNmpu1, COUNT_POLYGONS_BUFFER> PolygonsConnector;


/*!
 *  \brief Класс с информации о массиве используемом в функции nmglDrawArrays
 *  \author Жиленков Иван
 */
struct Array {
	const void* pointer;
	NMGLint size;
	NMGLint stride;
	NMGLenum type;
	NMGLboolean enabled;
	int offset;
};

struct ImageSegments{
	Rectangle rectangles[40];
	v2nm32f lowerLeft[40];
	v2nm32f upperRight[40];
	int count;
	int dummy;
};


/*!
 *  \brief Класс с информацией о делении изображения на сегменты
 *  \author Жиленков Иван
 */
struct WindowInfo {
	ImageSegments segments;
	float viewportMulX;
	float viewportMulY;
	float viewportMulZ;
	float viewportAddX;
	float viewportAddY;
	float viewportAddZ;
	Size imageSize;
};

struct NMGL_ScissorTest {
	ImageSegments segments;
	Vector2 origin;
	Size size;
	int isEnabled;
	int dummy;
};
#endif
