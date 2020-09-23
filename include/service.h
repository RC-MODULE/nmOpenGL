#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmtype.h"

typedef v4nm32u abgr32;

/**
\defgroup convertABGR32_RGB565_f convertABGR32_RGB565
\brief Преобразование формата ABGR32-32-32-32 в формат RGB565 
\param srcArray [in] Входной массив элементов в формате ABGR32
\param dstArray [in] Выходной массив элементов в формате RGB565
\param count [in] Количество элементов во входном массиве для обработки 
\return Через параметр возвращает преобразованные элементы в формате RGB565
\par
\xmlonly
	<testperf>
 		<param name=" srcArray "> im0 im1 </param>
 		<param name=" dstArray "> im2 im3 </param>
 		<param name=" count "> 4 </param>
	</testperf>
\endxmlonly
*/
//! \{
extern "C" void convertABGR32_RGB565(const abgr32  *srcArray, rgb565 *dstArray, int count);
//! \}

/**
\defgroup convertRGB565_RGB8888_f convertRGB565_RGB8888
\brief Преобразование формата RGB565 в формат RGB8888 
\param srcArray [in] Входной массив элементов в формате RGB565
\param dstArray [in] Выходной массив элементов в формате RGB8888
\param count [in] Количество элементов во входном массиве для обработки 
\return Через параметр возвращает преобразованные элементы в формате RGB8888
\par
\xmlonly
	<testperf>
 		<param name=" srcArray "> im0 im1 </param>
 		<param name=" dstArray "> im2 im3 </param>
 		<param name=" count "> 4 </param>
	</testperf>
\endxmlonly
*/
//! \{
extern "C" void convertRGB565_RGB8888(const rgb565 *srcArray, rgb8888 *dstArray, int count);
//! \}

/**
\defgroup vertexPrimitiveRepack_f vertexPrimitiveRepack
\brief Переупаковка вершин в формат GL_TRIANGLES 
\param srcVertex [in] Входной массив вершин
\param srcColor [in] Входной массив цветов 
\param dstVertex [out] Выходной массив вершин
\param dstColor [out] Выходной массив цветов 
\param mode [in] Входной формат треугольных примитивов
\param vertCount [in] Количество вершин во входных массивах для обработки 
\return Возвращает количество треугольников в выходных массивах
\par
\xmlonly
	<testperf>
 		<param name=" srcVertex "> im0 </param>
 		<param name=" srcColor "> im0 </param>
 		<param name=" dstVertex "> im0 </param>
 		<param name=" dstColor "> im0 </param>
 		<param name=" mode "> NMGL_TRIANGLES </param>
 		<param name=" vertCount "> 192 48 </param>
	</testperf>
	<testperf>
 		<param name=" srcVertex "> im0 </param>
 		<param name=" srcColor "> im0 </param>
 		<param name=" dstVertex "> im0 </param>
 		<param name=" dstColor "> im0 </param>
 		<param name=" mode "> NMGL_TRIANGLE_STRIP </param>
 		<param name=" vertCount "> 66 34 </param>
	</testperf>
	<testperf>
 		<param name=" srcVertex "> im0 </param>
 		<param name=" srcColor "> im0 </param>
 		<param name=" dstVertex "> im0 </param>
 		<param name=" dstColor "> im0 </param>
 		<param name=" mode "> NMGL_TRIANGLE_FAN </param>
 		<param name=" vertCount "> 66 34 </param>
	</testperf>
\endxmlonly
*/
//! \{
extern "C" int vertexPrimitiveRepack(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int mode, int vertCount);
//! \}

/**
\defgroup nmppsCmpGteLteMirrorV_32f_f nmppsCmpGteLteMirrorV_32f
\brief Проверка принадлежности элементов одного массива соответствующим  
\ диапазонам, заданным вторым массивом
\param srcArray [in] Входной массив элементов
\param wArray [in] Входной массив, задающий диапазоны
\param evenFlags [out] Выходной массив флагов принадлежности чётных элементов
\param oddFlags [out] Выходной массив флагов принадлежности нечётных элементов
\param size [in] Количество элементов во входном массиве 
\return Через параметры возвращает флаги принадлежности диапазонам
\par
\xmlonly
	<testperf>
 		<param name=" srcArray "> im0 im1 im2 im3 im4 im5 </param>
 		<param name=" wArray "> im1 im2 im3 im4 im5 </param>
 		<param name=" evenFlags "> im1 im2 im3 im4 im5 </param>
 		<param name=" oddFlags "> im1 im2 im3 im4 im5 </param>
 		<param name=" size "> 64 </param>
	</testperf>
\endxmlonly
*/
//! \{
extern "C" void nmppsCmpGteLteMirrorV_32f(const nm32f *srcArray, const nm32f *wArray, nm1* evenFlags, nm1* oddFlags, int size);
//! \}

// Declarations for the triangulate function:
struct Point {
	nm32f x;
	nm32f y;
	nm32f z;
	v4nm32f color;
	Point() { x = 0; y = 0; z = 0; color = { 0, 0, 0, 0 }; }
	Point(nm32f xx, nm32f yy, nm32f zz, v4nm32f col) : x(xx), y(yy), z(zz), color(col) {}
};

struct Edge {
	int p1;
	int p2;
	Edge() {};
	Edge(int pp1, int pp2): p1(pp1), p2(pp2) {};
};

struct edgeProjection{
	nm32f dx;
	nm32f dy;
};

struct Triangle {
	Point points[3];
	Edge edges[3] = {{0, 1}, {1, 2}, {0, 2}};
	
	Triangle() {};
	Triangle(Point a, Point b, Point c) : points{a, b, c} {};
	
	nm32f edgeSize(int i) const;
	edgeProjection edgeGetProjection(int i) const;
};

typedef struct Vertices 
{
	nm32f v[9];
} Vertices;

typedef struct Colors 
{
	v4nm32f c[3];
} Colors;

// Data structure to store Vertices and Colors in 
// "data" buffer (allocated previously)
typedef struct Buffer 
{
	int back;
	int front;
	int size;
	TrianglePointers *data;
} Buffer;

//static Buffer initBuf(void *data, int size);
static Buffer initBuf(TrianglePointers *trPtr, int size);
static int bufIsFull(Buffer *buf);
static int bufIsEmpty(Buffer *buf);
static int bufSize(Buffer *buf);
static int bufSpace(Buffer *buf);
//static int pushBackVertices(Buffer *vbuf, Vertices *vert);
//static int pushFrontVertices(Buffer *vbuf, Vertices *vert);
//static int popBackVertices(Buffer *vbuf, Vertices *vert);
//static int pushBackColors(Buffer *cbuf, Colors *colors);
//static int pushFrontColors(Buffer *cbuf, Colors *colors);
//static int popBackColors(Buffer *cbuf, Colors *colors);
static int pushBack(Buffer *buf, Vertices *vert, Colors *colors);
static int popBack(Buffer *buf, Vertices *vert, Colors *colors);
static int pushFront(Buffer *buf, Vertices *vert, Colors *colors);

int triangulateOneTriangle(const Triangle& tr, nm32f xMax, nm32f yMax, Buffer *buf);


/*
 * Init function for performance test
 */
static void srcVertexInit(nm32f *srcVertex, int srcCount)
{
	nm32f srcVertexInitArray[] = {
								1,
								1,
								4,
								2,
								2,
								7,
								3,
								1,
								9
							};

	int coordCount = 9 * srcCount;

	for (int i = 0; i < coordCount; ++i){
		srcVertex[i] = srcVertexInitArray[i];
	}
}

/**
\defgroup triangulate_f triangulate
\brief Разбиение большого треугольника на маленькие (триангуляция)
\param srcVertex [in] Массив вершин входных треугольников
\param srcColor [in] Массив цветов входных треугольников
\param srcCount [in] Количество входных треугольников
\param maxWidth [in] Максимально допустимая ширина треугольника
\param maxHeight [in] Максимально допустимая высота треугольника
\param maxDstSize [in] Максимально допустимое число выходных треугольников 
\param dstVertex [out] Массив вершин выходных треугольников
\param dstColor [out] Массив цветов выходных треугольников
\param srcTreatedCount [out] Количество обработанных входных треугольников 
\return Итоговое количество выходных треугольников
\par
\xmlonly
	<testperf>
 		<param name=" srcVertex "> im0 </param>
 		<param name=" srcColor "> im1 </param>
 		<param name=" srcCount "> 1 </param>
 		<param name=" maxWidth "> 1 </param>
 		<param name=" maxHeight "> 1 </param>
 		<param name=" maxDstSize "> 10 </param>
 		<param name=" dstVertex "> im2 </param>
 		<param name=" dstColor "> im3 </param>
 		<param name=" srcTreatedCount "> im4 </param>
		<init>
			srcVertexInit(srcVertex, 1);
		</init>
		<size> srcCount </size>
	</testperf>
\endxmlonly
*/
//! \{
int triangulate(const nm32f *srcVertex, const v4nm32f *srcColor, int srcCount, int maxWidth, int maxHeight, int maxDstSize, nm32f *dstVertex, v4nm32f *dstColor, int *srcTreatedCount);
//! \}

#endif //__SERVICE_H__
