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

#include <vector>

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
	Triangle(Point a, Point b, Point c) : points{a, b, c} {}
	
	void print();
	nm32f edgeSize(int i) const;
};

typedef struct Vertices {
	nm32f ax;
	nm32f ay;
	nm32f az;
	nm32f bx;
	nm32f by;
	nm32f bz;
	nm32f cx;
	nm32f cy;
	nm32f cz;
} Vertices;

int triangulateOneTriangle(const Triangle &tr, nm32f xMax, nm32f yMax, int trLimit, std::vector<Triangle>& trVec);
int triangulate(const nm32f *srcVertex, const v4nm32f *srcColor, int srcCount, int maxWidth, int maxHeight, int maxDstSize, nm32f *dstVertex, v4nm32f *dstColor, int *srcTreatedCount);

#endif //__SERVICE_H__
