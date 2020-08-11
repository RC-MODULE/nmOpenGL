#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "demo3d_common.h"
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

extern "C" void convertRGB565_RGB8888(const rgb565 *srcArray, rgb8888 *dstArray, int count);
extern "C" void nmppsCmpGteLteMirrorV_32f(const nm32f *srcArray, const nm32f *wArray, nm1* evenFlags, nm1* oddFlags, int size);
extern "C" int vertexPrimitiveRepack(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int mode, int vertCount);

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

struct Triangle {
	Point points[3];
	Edge edges[3] = {{0, 1}, {1, 2}, {0, 2}};
	
	Triangle() {};
	Triangle(Point a, Point b, Point c) : points{a, b, c} {};
	
	nm32f edgeSize(int i) const;
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
	void *data;
} Buffer;

static Buffer initBuf(void *data, int size);
static int bufIsFull(Buffer *buf);
static int bufIsEmpty(Buffer *buf);
static int bufSize(Buffer *buf);
static int bufSpace(Buffer *buf);
static int pushBackVertices(Buffer *vbuf, Vertices *vert);
static int pushFrontVertices(Buffer *vbuf, Vertices *vert);
static int popBackVertices(Buffer *vbuf, Vertices *vert);
static int pushBackColors(Buffer *cbuf, Colors *colors);
static int pushFrontColors(Buffer *cbuf, Colors *colors);
static int popBackColors(Buffer *cbuf, Colors *colors);

int triangulateOneTriangle(const Triangle& tr, nm32f xMax, nm32f yMax, Buffer *verticesStack, Buffer *colorsStack);
int triangulate(const nm32f *srcVertex, const v4nm32f *srcColor, int srcCount, int maxWidth, int maxHeight, int maxDstSize, nm32f *dstVertex, v4nm32f *dstColor, int *srcTreatedCount);

#endif //__SERVICE_H__
