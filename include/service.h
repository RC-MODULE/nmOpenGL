#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "demo3d_common.h"
#include "demo3d_nm0.h"
#include "nmtype.h"

typedef v4nm32u abgr32;

/*!
\ingroup service_api
\defgroup convertABGR32_RGB565 convertABGR32_RGB565 
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
\defgroup convertRGB565_RGB8888 convertRGB565_RGB8888 
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

static void initDstTrianglePointers(TrianglePointers *dst, int outputTrianglesCount)
{
	const int ATTR_PER_VERTEX = 10;
	nm32f *buf = (nm32f *)(dst + 1);
	dst->v0.x = buf;
	dst->v0.y = dst->v0.x + outputTrianglesCount;
	dst->v0.z = dst->v0.y + outputTrianglesCount;
	dst->v0.w = dst->v0.z + outputTrianglesCount;
	dst->v0.s = dst->v0.w + outputTrianglesCount;
	dst->v0.t = dst->v0.s + outputTrianglesCount;
    dst->v0.color = (v4nm32f *)(dst->v0.t + outputTrianglesCount);

	dst->v1.x = buf + ATTR_PER_VERTEX * outputTrianglesCount;
	dst->v1.y = dst->v1.x + outputTrianglesCount;
	dst->v1.z = dst->v1.y + outputTrianglesCount;
	dst->v1.w = dst->v1.z + outputTrianglesCount;
	dst->v1.s = dst->v1.w + outputTrianglesCount;
	dst->v1.t = dst->v1.s + outputTrianglesCount;
    dst->v1.color = (v4nm32f *)(dst->v1.t + outputTrianglesCount);
	
	dst->v2.x = buf + ATTR_PER_VERTEX * 2 * outputTrianglesCount;
	dst->v2.y = dst->v2.x + outputTrianglesCount;
	dst->v2.z = dst->v2.y + outputTrianglesCount;
	dst->v2.w = dst->v2.z + outputTrianglesCount;
	dst->v2.s = dst->v2.w + outputTrianglesCount;
	dst->v2.t = dst->v2.s + outputTrianglesCount;
    dst->v2.color = (v4nm32f *)(dst->v2.t + outputTrianglesCount);
}

/*!
 *  \ingroup service_api
 *  \defgroup repackToPrimitives_t repackToPrimitives_t
 *  \brief Функции переупаковки вершин TRIANGLES
 *  \author Жиленков Иван
 *
 *  \param srcVertex [in] Входной массив вершин
 *  \param srcColor [in] Входной массив цветов 
 *  \param srcTex [in] Входной массив текстурных координат 
 *  \param dstVertex [out] Выходной массив треугольников
 *  \param vertexAmount [in] Количество вершин во входных массивах для обработки 
 *  \return Возвращает количество треугольников в выходном массиве
 *  \details Details
 *  \par
 *  \xmlonly
 *  	<testperf>
 *   		<param name=" srcVertex "> im10 </param>
 *   		<param name=" srcColor ">  im20 </param>
 *   		<param name=" srcTex ">    im30 </param>
 *   		<param name=" dstVertex "> im40 </param>
 *   		<param name=" vertexAmount "> 48 96 192 384 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  	<testperf>
 *   		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 *   		<param name=" srcColor ">  im01 im11 im21 im31 im41 im51 im61 im71 </param>
 *   		<param name=" srcTex ">    im02 im12 im22 im32 im42 im52 im62 im72 </param>
 *			<param name=" dstVertex "> im03 im13 im23 im33 im43 im53 im63 im73 </param>
 *   		<param name=" vertexAmount "> 48 96 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  \endxmlonly
 */
 //! \{
extern "C" int repackToPrimitives_t(const v4nm32f *srcVertex,
	const v4nm32f *srcColor,
	const v2nm32f *srcTex,
	TrianglePointers *dstVertex,
	int vertexAmount);
 //! \}

/*!
 *  \ingroup service_api
 *  \defgroup repackToPrimitives_ts repackToPrimitives_ts
 *  \brief Функции переупаковки вершин TRIANGLE_STRIP
 *
 *  \param srcVertex [in] Входной массив вершин
 *  \param srcColor [in] Входной массив цветов 
 *  \param srcTex [in] Входной массив текстурных координат 
 *  \param dstVertex [out] Выходной массив треугольников
 *  \param vertexAmount [in] Количество вершин во входных массивах для обработки 
 *  \return Возвращает количество треугольников в выходном массиве
 *  \details Details
 *  \par
 *  \xmlonly
 *  	<testperf>
 *   		<param name=" srcVertex "> im10 </param>
 *   		<param name=" srcColor ">  im20 </param>
 *   		<param name=" srcTex ">    im30 </param>
 *   		<param name=" dstVertex "> im40 </param>
 *   		<param name=" vertexAmount "> 34 66 136 264 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  	<testperf>
 *   		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 *   		<param name=" srcColor ">  im01 im11 im21 im31 im41 im51 im61 im71 </param>
 *   		<param name=" srcTex ">    im02 im12 im22 im32 im42 im52 im62 im72 </param>
 *			<param name=" dstVertex "> im03 im13 im23 im33 im43 im53 im63 im73 </param>
 *   		<param name=" vertexAmount "> 34 66 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  \endxmlonly
 */
 //! \{
extern "C" int repackToPrimitives_ts(const v4nm32f *srcVertex,
	const v4nm32f *srcColor,
	const v2nm32f *srcTex,
	TrianglePointers *dstVertex,
	int vertexAmount);
 //! \}

/*!
 *  \ingroup service_api
 *  \defgroup repackToPrimitives_tf repackToPrimitives_tf
 *  \brief Функции переупаковки вершин треугольников TRIANGLE_FAN
 *
 *  \param srcVertex [in] Входной массив вершин
 *  \param srcColor [in] Входной массив цветов 
 *  \param srcTex [in] Входной массив текстурных координат 
 *  \param dstVertex [out] Выходной массив треугольников
 *  \param vertexAmount [in] Количество вершин во входных массивах для обработки 
 *  \return Возвращает количество треугольников в выходном массиве
 *  \details Details
 *  \par
 *  \xmlonly
 *  	<testperf>
 *   		<param name=" srcVertex "> im10 </param>
 *   		<param name=" srcColor ">  im20 </param>
 *   		<param name=" srcTex ">    im30 </param>
 *   		<param name=" dstVertex "> im40 </param>
 *   		<param name=" vertexAmount "> 34 66 136 264 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  	<testperf>
 *   		<param name=" srcVertex "> im00 im10 im20 im30 im40 im50 im60 im70 </param>
 *   		<param name=" srcColor ">  im01 im11 im21 im31 im41 im51 im61 im71 </param>
 *   		<param name=" srcTex ">    im02 im12 im22 im32 im42 im52 im62 im72 </param>
 *			<param name=" dstVertex "> im03 im13 im23 im33 im43 im53 im63 im73 </param>
 *   		<param name=" vertexAmount "> 34 66 </param>
 *			<init>
 *				initDstTrianglePointers(dstVertex, vertexAmount);
 *			</init>
 *			<size> vertexAmount </size>
 *  	</testperf>
 *  \endxmlonly
 */
 //! \{
extern "C" int repackToPrimitives_tf(const v4nm32f *srcVertex,
	const v4nm32f *srcColor,
	const v2nm32f *srcTex,
	TrianglePointers *dstVertex,
	int vertexAmount);
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
	nm32f GetWidth(void) const;
	nm32f GetHeight(void) const;
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
//int triangulateOneTriangle(const Triangle &tr, nm32f xMax, nm32f yMax, Buffer *verticesStack, Buffer *colorsStack);
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

extern "C" int buildMatrix(nm32f *x, nm32f *y, nm32f *z, const nm32f *ab_dxy, const nm32f *dz, int k);
extern "C" int splitTrs(TrianglePointers *inTrs, int srcCount, int maxWidth, int maxHeight, int maxDstSize, TrianglePointers *outTrs, int *srcTreatedCount);
extern "C" void nmppsConvert_32f32s_ceil(const nm32f* pSrcVec, nm32f* pDstVec, int nSize);

#endif //__SERVICE_H__
