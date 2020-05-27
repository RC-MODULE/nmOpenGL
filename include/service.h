#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "demo3d_common.h"
#include "nmtype.h"

typedef v4nm32u abgr32;

extern "C" {

	void convertABGR32_RGB565(const abgr32  *srcArray, rgb565 *dstArray, int count);
	void convertRGB565_RGB8888(const rgb565 *srcArray, rgb8888 *dstArray, int count);
	void nmppsCmpGteLteMirrorV_32f(const nm32f *srcArray, const nm32f *wArray, nm1* evenFlags, nm1* oddFlags, int size);
	int vertexPrimitiveRepack(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int mode, int vertCount);

}

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

typedef struct Stack 
{
	int top;
	int front;
	int size;
	void *data;
} Stack;

typedef struct VertexBuf 
{
	int top;
	int front;
	int size;
	void *data;
} VertexBuf;

static void printStack(Stack *st, int itemSize);
static void printVertexBuf(VertexBuf *vbuf, int itemSize);


static VertexBuf createVertexBuf(void *data, int size)
{
	VertexBuf vertexBuf;
	vertexBuf.data = data;
	vertexBuf.top = size;
	vertexBuf.front = -1;
	vertexBuf.size = size;
	return vertexBuf;
}

static int vBufIsFull(VertexBuf *vbuf)
{
	return ((vbuf->front + 1) == vbuf->top);	
}

static int vBufIsEmpty(VertexBuf *vbuf)
{
	return (vbuf->top == vbuf->size);	
}

static int vBufSize(VertexBuf *vbuf)
{
	return (vbuf->front + 1);	
}

static int vBufSpace(VertexBuf *vbuf)
{
	return (vbuf->top - vbuf->front - 1);	
}
//      push here         
//          |
//
//          V
// |-|-|-|-|-|x|x|
static int vertexBufPushBack(VertexBuf *vbuf, Vertices *vert)
{
	if (vBufIsFull(vbuf)){
		return -1;
	} else {
		vbuf->top -= 1;
		nm32f *dst = (nm32f *) vbuf->data; 
		for (int i = 0; i < 9; ++i){
			dst[i * vbuf->size + vbuf->top] = vert->v[i];
		}	
		return 0;
	}
}

//      push here         
//          |
//          V
// |x|x|x|x|-|-|-|
static int vertexBufPushFront(VertexBuf *vbuf, Vertices *vert)
{
	if (vBufIsFull(vbuf)){
		return -1;
	} else {
		vbuf->front += 1;
		nm32f *dst = (nm32f *) vbuf->data; 
		for (int i = 0; i < 9; ++i){
			dst[i * vbuf->size + vbuf->front] = vert->v[i];
		}	
		return 0;
	}
}

//   return this item         
//          |
//          V
// |-|-|-|-|x|x|x|
static int vertexBufPopBack(VertexBuf *vbuf, Vertices *vert)
{
	if (vBufIsEmpty(vbuf)){
		return -1;
	} else {
		nm32f *src = (nm32f *) vbuf->data; 
		for (int i = 0; i < 9; ++i){
			vert->v[i] = src[i * vbuf->size + vbuf->top];
		}	
		vbuf->top += 1;
		return 0;
	}
}

static Stack createStack(void *data, int size)
{
	Stack st;
	st.data = data;
	st.top = size;
	st.front = -1;
	st.size = size;
	return st;
}

static int isFull(Stack *st)
{
	return ((st->front + 1) == st->top);	
}

static int isEmpty(Stack *st)
{
	return (st->top == st->size);	
}

static int stackSize(Stack *st)
{
	return (st->front + 1);	
}

static int space(Stack *st)
{
	return (st->top - st->front - 1);	
}
//      push here         
//          |
//
//          V
// |-|-|-|-|-|x|x|
static int pushBackColor(Stack *st, Colors *color)
{
	if (isFull(st)){
		return -1;
	} else {
		st->top -= 1;
		Colors *topColor = &((Colors *) st->data)[st->top];
		nm32f *dst = (nm32f *) topColor;
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 4; ++j){
				dst[i * 4 + j] = color->c[i].vec[j];
			}
		}
		return 0;
	}
}

//      push here         
//          |
//          V
// |x|x|x|x|-|-|-|
static int pushFrontColor(Stack *st, Colors *color)
{
	if (isFull(st)){
		return -1;
	} else {
		st->front += 1;
		Colors *frontColor = &((Colors *) st->data)[st->front];
		nm32f *dst = (nm32f *) frontColor;
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 4; ++j){
				dst[i * 4 + j] = color->c[i].vec[j];
			}
		}
		return 0;
	}
}

//   return this item         
//          |
//          V
// |-|-|-|-|x|x|x|
static int popBackColor(Stack *st, Colors *color)
{
	if (isEmpty(st)){
		return -1;
	} else {
		Colors *topColor = &((Colors *) st->data)[st->top];
		nm32f *src = (nm32f *) topColor;
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 4; ++j){
				color->c[i].vec[j] = src[i * 4 + j];
			}
		}
		st->top += 1;
		return 0;
	}
}

static void printStack(Stack *st)
{
#if 1 
	nm32f *data = (nm32f *) st->data;
	printf(" (free %i it) ", space(st));
	for (int i = 0; i < st->size * 12; ++i){
		printf("%3.2f ", data[i]);
	}
	puts("");
#endif
}

static void printVertexBuf(VertexBuf *vbuf, int itemSize)
{
#if 1 
	nm32f *data = (nm32f *) vbuf->data;
	printf(" (free %i it):\n\r", vBufSpace(vbuf));
	for (int i = 0; i < 9; ++i){
		for (int j = 0; j < vbuf->size; ++j){
			printf("%3.2f ", data[i * vbuf->size + j]);
		}
		puts("");
	}
	puts("");
#endif
}

//int triangulateOneTriangle(const Triangle &tr, nm32f xMax, nm32f yMax, int trLimit, std::vector<Triangle>& trVec);
//int triangulateOneTriangle(const Triangle& tr, nm32f xMax, nm32f yMax, void *dstVertexBuf, void *dstColorBuf, int bufSize);
int triangulateOneTriangle(const Triangle& tr, nm32f xMax, nm32f yMax, VertexBuf *verticesStack, Stack *colorsStack);
int triangulate(const nm32f *srcVertex, const v4nm32f *srcColor, int srcCount, int maxWidth, int maxHeight, int maxDstSize, nm32f *dstVertex, v4nm32f *dstColor, int *srcTreatedCount);

#endif //__SERVICE_H__
