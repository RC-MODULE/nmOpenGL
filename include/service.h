#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmtype.h"

typedef v4nm32u abgr32;

/*!
\ingroup service_api
\brief Преобразование формата ABGR32-32-32-32 в формат RGB565 
\param srcArray [in] Входной массив элементов в формате ABGR32
\param dstArray [in] Выходной массив элементов в формате RGB565
\param count [in] Количество элементов во входном массиве для обработки 

\return Через параметр возвращает преобразованные элементы в формате RGB565

\par
\xmlonly
	<testperf>
 		<param name=" srcArray "> im00 </param>
 		<param name=" dstArray "> im10 </param>
 		<param name=" count "> 4 8 12 16 20 24 28 32 36 40 128 256 512 </param>
	</testperf>
	<testperf>
 		<param name=" srcArray "> im00 im10 im20 im30 </param>
 		<param name=" dstArray "> im01 im11 im21 im31 </param>
 		<param name=" count "> 4 </param>
	</testperf>
\endxmlonly
*/
//! \{
extern "C" void convertABGR32_RGB565(const abgr32  *srcArray, rgb565 *dstArray, int count);
//! \}

/*!
\ingroup service_api
\brief Преобразование формата RGB565 в формат RGB8888 
\details Количество обрабатываемых элементов должно быть кратно 4, в противном случае
оно округляется до кратного 4 в нижнюю сторону (например, если count = 5, 
то обработано будет 4 элемента).
\param srcArray [in] Входной массив элементов в формате RGB565
\param dstArray [in] Выходной массив элементов в формате RGB8888
\param count [in] Количество элементов во входном массиве для обработки 

\return Через параметр возвращает преобразованные элементы в формате RGB8888

\par
\xmlonly
	<testperf>
 		<param name=" srcArray "> im00 </param>
 		<param name=" dstArray "> im10 </param>
 		<param name=" count "> 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 36 40 128 256 512 1024 2048 4096 </param>
	</testperf>
	<testperf>
 		<param name=" srcArray "> im00 im10 im20 im30 </param>
 		<param name=" dstArray "> im01 im11 im21 im31 </param>
 		<param name=" count "> 4 </param>
	</testperf>
\endxmlonly
*/
//! \{
extern "C" void convertRGB565_RGB8888(const rgb565 *srcArray, rgb8888 *dstArray, int count);
//! \}

/*!
\ingroup service_api
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
 		<param name=" srcVertex "> im00 </param>
 		<param name=" srcColor ">  im10 </param>
 		<param name=" dstVertex "> im20 </param>
 		<param name=" dstColor ">  im30 </param>
 		<param name=" mode "> NMGL_TRIANGLES </param>
 		<param name=" vertCount "> 48 96 192 384 </param>
	</testperf>
	<testperf>
 		<param name=" srcVertex "> im00 </param>
 		<param name=" srcColor ">  im10 </param>
 		<param name=" dstVertex "> im20 </param>
 		<param name=" dstColor ">  im30 </param>
 		<param name=" mode "> NMGL_TRIANGLE_STRIP </param>
 		<param name=" vertCount "> 34 66 136 264 </param>
	</testperf>
	<testperf>
 		<param name=" srcVertex "> im00 </param>
 		<param name=" srcColor ">  im10 </param>
 		<param name=" dstVertex "> im20 </param>
 		<param name=" dstColor ">  im30 </param>
 		<param name=" mode "> NMGL_TRIANGLE_FAN </param>
 		<param name=" vertCount "> 34 66 136 264 </param>
	</testperf>
	<testperf>
 		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 		<param name=" srcColor ">  im01 im11 im21 im31 im41 im51 im61 im71 </param>
 		<param name=" dstVertex "> im02 im12 im22 im32 im42 im52 im62 im72 </param>
 		<param name=" dstColor ">  im03 im13 im23 im33 im43 im53 im63 im73 </param>
 		<param name=" mode "> NMGL_TRIANGLES </param>
 		<param name=" vertCount "> 48 96 </param>
	</testperf>
	<testperf>
 		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 		<param name=" srcColor ">  im01 im11 im21 im31 im41 im51 im61 im71 </param>
 		<param name=" dstVertex "> im02 im12 im22 im32 im42 im52 im62 im72 </param>
 		<param name=" dstColor ">  im03 im13 im23 im33 im43 im53 im63 im73 </param>
 		<param name=" mode "> NMGL_TRIANGLE_STRIP </param>
 		<param name=" vertCount "> 34 66 </param>
	</testperf>
	<testperf>
 		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 		<param name=" srcColor ">  im01 im11 im21 im31 im41 im51 im61 im71 </param>
 		<param name=" dstVertex "> im02 im12 im22 im32 im42 im52 im62 im72 </param>
 		<param name=" dstColor ">  im03 im13 im23 im33 im43 im53 im63 im73 </param>
 		<param name=" mode "> NMGL_TRIANGLE_FAN </param>
 		<param name=" vertCount "> 34 66 </param>
	</testperf>
\endxmlonly
*/
//! \{
extern "C" int vertexPrimitiveRepack(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int mode, int vertCount);
//! \}

/**
\defgroup texCoordsRepack_f texCoordsRepack
\ingroup service_api
\brief Переупаковка текстурных координат аналогично переупаковке вершин vertexPrimitiveRepack
\param srcTexCoords[in] Входной массив текстурных координат 
\param dstTexCoords[out] Выходной массив текстурных координат 
\param mode [in] Входной формат треугольных примитивов
\param vertCount [in] Количество вершин во входном массиве для обработки 
\return Возвращает количество треугольников, cooтветствующее количеству текстурных координат в выходном массиве
\par
*/
//! \{
extern "C" int texCoordsRepack(const v2nm32f *srcTexCoords,  nm32f *dstTexCoords, int mode, int vertCount);
//! \}

/**
\defgroup nmppsCmpGteLteMirrorV_32f_f nmppsCmpGteLteMirrorV_32f
\ingroup service_api
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
 		<param name=" srcArray ">	im00 </param>
 		<param name=" wArray ">		im10 </param>
 		<param name=" evenFlags ">	im20 </param>
 		<param name=" oddFlags ">	im30 </param>
 		<param name=" size "> 64 128 192 256 512 768 1024 1280 1536 1792 2048 </param>
	</testperf>
	<testperf>
 		<param name=" srcArray ">	im00 im10 im20 im30 im40 im50 im60 im70 </param>
 		<param name=" wArray ">		im01 im11 im21 im31 im41 im51 im61 im71 </param>
 		<param name=" evenFlags ">	im02 im12 im22 im32 im42 im52 im62 im72 </param>
 		<param name=" oddFlags ">	im03 im13 im23 im33 im43 im53 im63 im73 </param>
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
    nm32f s;
    nm32f t;
    nm32f w;
	Point() { x = 0; y = 0; z = 0; color = { 0, 0, 0, 0 }; s = 0; t = 0; w = 0;}
	Point(nm32f xx, nm32f yy, nm32f zz, v4nm32f col, nm32f ss, nm32f tt, nm32f ww) : x(xx), y(yy), z(zz), color(col), s(ss), t(tt), w(ww) {}
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
	Edge edges[3] = {{0, 1}, {1, 2}, {2, 0}};
	
	Triangle() {};
	Triangle(Point a, Point b, Point c) : points{a, b, c} {};
	
	nm32f edgeSize(int i) const;
	bool isTooBig(nm32f xMax, nm32f yMax) const;
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

typedef struct Texcoords
{
	nm32f st[6];
} Texcoords;

typedef struct Wclips
{
	nm32f w[3];
} Wclips;

// Data structure to store Vertices and Colors in 
// "data" buffer (allocated previously)
typedef struct Buffer 
{
	int back;
	int front;
	int size;
	void *data;
} Buffer;

static Buffer initBuf(void *data, int size);
// Common buffer functions
static int bufIsFull(Buffer *buf);
static int bufIsEmpty(Buffer *buf);
static int bufSize(Buffer *buf);
static int bufSpace(Buffer *buf);
// functions for splitTriangles
static int pushBack(Buffer *buf, Vertices *vert, Colors *colors);
static int popBack(Buffer *buf, Vertices *vert, Colors *colors);
static int pushFront(Buffer *buf, Vertices *vert, Colors *colors);
// functions for triangulate
static int pushBackVertices(Buffer *vbuf, Vertices *vert);
static int pushFrontVertices(Buffer *vbuf, Vertices *vert);
static int popBackVertices(Buffer *vbuf, Vertices *vert);
static int pushBackColors(Buffer *cbuf, Colors *colors);
static int pushFrontColors(Buffer *cbuf, Colors *colors);
static int popBackColors(Buffer *cbuf, Colors *colors);
static int pushBackTexcoords(Buffer *stbuf, Texcoords *texcoords);
static int pushFrontTexcoords(Buffer *stbuf, Texcoords *texcoords);
static int popBackTexcoords(Buffer *stbuf, Texcoords *texcoords);
static int pushBackWclips(Buffer *wbuf, Wclips *wclips);
static int pushFrontWclips(Buffer *wbuf, Wclips *wclips);
static int popBackWclips(Buffer *wbuf, Wclips *wclips);

int triangulateOneTriangle(const Triangle& tr, nm32f xMax, nm32f yMax, Buffer *verticesStack, Buffer *colorsStack, Buffer *texcoordsStack, Buffer *wclipsStack);
int splitOneTriangle(const Triangle& tr, nm32f xMax, nm32f yMax, Buffer *buf);

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
	// Put srcCount similar triangles into srcVertex
	for (int i = 0; i < srcCount; ++i){
		for (int j = 0; j < coordCount; ++j){
			srcVertex[9 * i + j] = srcVertexInitArray[j];
		}
	}
}

// Macro PERSPECTIVE_CORRECT_TRIANGULATION should be enabled when 
// perspective correct interpolation of s,t,w is needed during texturing.
// Later, code under this macro may be used in glHint(GL_PERSPECTIVE_CORRECTION_HINT) implementation.

//#define PERSPECTIVE_CORRECT_TRIANGULATION //code under this macro should be used in glHint(define this macro 

/**
\ingroup service_api
\brief Разбиение большого треугольника на маленькие (триангуляция)
\param srcVertex [in] Массив вершин входных треугольников
\param srcColor [in] Массив цветов входных треугольников
\param srcTexcoords [in] Массив текстурных координат входных треугольников
\param srcWclip [in] Массив координат w в пространстве отсечения (clip) входных треугольников
\param srcCount [in] Количество входных треугольников
\param maxWidth [in] Максимально допустимая ширина треугольника
\param maxHeight [in] Максимально допустимая высота треугольника
\param maxDstSize [in] Максимально допустимое число выходных треугольников 
\param dstVertex [out] Массив вершин выходных треугольников
\param dstColor [out] Массив цветов выходных треугольников
\param dstTexcoords [out] Массив текстурных координат выходных треугольников
\param dstWclip [out] Массив координат w в пространстве отсечения (clip) выходных треугольников
\param srcTreatedCount [out] Количество обработанных входных треугольников 
\return Итоговое количество выходных треугольников
\par
\xmlonly
	<testperf>
 		<param name=" srcVertex "> im00 </param>
 		<param name=" srcColor "> im10 </param>
 		<param name=" srcTexcoords "> im40 </param>
 		<param name=" srcWclip "> im50 </param>
 		<param name=" srcCount "> 1 2 3 4 5 6 8 8 9 10 11 12 13 14 15 16 17 18 19 20 </param>
 		<param name=" maxWidth "> 2 </param>
 		<param name=" maxHeight "> 2 </param>
 		<param name=" maxDstSize "> 10 </param>
 		<param name=" dstVertex "> im20 </param>
 		<param name=" dstColor "> im30 </param>
 		<param name=" dstTexcoords "> im60 </param>
 		<param name=" dstWclip "> im70 </param>
 		<param name=" srcTreatedCount "> im0 </param>
		<init>
			srcVertexInit(srcVertex, 20);
		</init>
		<size> srcCount </size>
	</testperf>
	<testperf>
 		<param name=" srcVertex "> im00 </param>
 		<param name=" srcColor "> im10 </param>
 		<param name=" srcTexcoords "> im40 </param>
 		<param name=" srcWclip "> im50 </param>
 		<param name=" srcCount "> 1 2 3 4 5 6 8 8 9 10 11 12 13 14 15 16 17 18 19 20 </param>
 		<param name=" maxWidth "> 1 </param>
 		<param name=" maxHeight "> 1 </param>
 		<param name=" maxDstSize "> 10 </param>
 		<param name=" dstVertex "> im20 </param>
 		<param name=" dstColor "> im30 </param>
 		<param name=" dstTexcoords "> im60 </param>
 		<param name=" dstWclip "> im70 </param>
 		<param name=" srcTreatedCount "> im0 </param>
		<init>
			srcVertexInit(srcVertex, 20);
		</init>
		<size> srcCount </size>
	</testperf>
	<testperf>
 		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 		<param name=" srcColor "> im01 im11 im21 im31 im41 im51 im61 im71 </param>
		<param name=" srcTexcoords "> im04 im14 im24 im34 im44 im54 im64 im74 </param>
		<param name=" srcWclip "> im05 </param>
 		<param name=" srcCount "> 1 </param>
 		<param name=" maxWidth "> 1 </param>
 		<param name=" maxHeight "> 1 </param>
 		<param name=" maxDstSize "> 10 </param>
 		<param name=" dstVertex "> im02 im12 im22 im32 im42 im52 im62 im72 </param>
 		<param name=" dstColor "> im03 im13 im23 im33 im43 im53 im63 im73 </param>
		<param name=" dstTexcoords "> im06 im16 im26 im36 im46 im56 im66 im76 </param>
		<param name=" dstWclip "> im07 </param>
 		<param name=" srcTreatedCount "> im0 im1 im2 im3 im4 im5 im6 im7 </param>
		<init>
			srcVertexInit(srcVertex, 1);
		</init>
		<size> srcCount </size>
	</testperf>
\endxmlonly
*/
//! \{
int triangulate(const nm32f *srcVertex, const v4nm32f *srcColor, const nm32f *srcTexcoords, const nm32f *srcWclip, int srcCount, int maxWidth, int maxHeight, int maxDstSize, nm32f *dstVertex, v4nm32f *dstColor, nm32f *dstTexcoords, nm32f *dstWclip, int *srcTreatedCount);
//! \}

#endif //__SERVICE_H__
